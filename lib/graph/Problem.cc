#include <disc/graph/Problem.h>

#include <cassert>

#include <array>
#include <iostream>

#include <glpk.h>

namespace disc {

  std::vector<double> computePii(const Matrix<double>& coeffs, const char *filename) {
    assert(coeffs.getCols() == coeffs.getRows());

    std::cout << "computing pi_i\n";

    std::size_t size = coeffs.getCols();

    glp_prob *prob = glp_create_prob();
    glp_set_prob_name(prob, "Pi_i");

    glp_set_obj_dir(prob, GLP_MAX);
    glp_set_obj_name(prob, "p_min");

    std::array<char, 128> buffer;

    // rows

    std::size_t nrows = size + 1;
    glp_add_rows(prob, nrows);

    for (std::size_t i = 0; i < size; ++i) {
      int row = i + 1;
      std::snprintf(buffer.data(), buffer.size(), "p_%i", (row - 1));
      glp_set_row_name(prob, row, buffer.data());
      glp_set_row_bnds(prob, row, GLP_LO, 0., 0.);
    }

    glp_set_row_name(prob, nrows, "probability");
    glp_set_row_bnds(prob, nrows, GLP_FX, 1., 1.);

    // cols

    auto ncols = size + 1;
    glp_add_cols(prob, ncols);

    for (std::size_t i = 0; i < size; ++i) {
      int col = i + 1;
      std::snprintf(buffer.data(), buffer.size(), "pi_%i", (col - 1));
      glp_set_col_name(prob, col, buffer.data());
      glp_set_col_bnds(prob, col, GLP_LO, 0., 0.);
      glp_set_obj_coef(prob, col, 0.0);
    }

    glp_set_col_name(prob, ncols, "p_min");
    glp_set_col_bnds(prob, ncols, GLP_LO, 0., 0.);
    glp_set_obj_coef(prob, ncols, 1.0);

    // matrix

    std::vector<int> ia;
    std::vector<int> ja;
    std::vector<double> ar;

    // index 0 is not used by glp
    ia.push_back(0);
    ja.push_back(0);
    ar.push_back(0.);

    for (std::size_t j = 0; j < size; ++j) {
      for (std::size_t i = 0; i < size; ++i) {
        ia.push_back(i + 1);
        ja.push_back(j + 1);
        ar.push_back(coeffs(i, j));
      }
    }

    for (std::size_t k = 0; k < size; ++k) {
      ia.push_back(nrows);
      ja.push_back(k + 1);
      ar.push_back(1.0);

      ia.push_back(k + 1);
      ja.push_back(ncols);
      ar.push_back(-1.0);
    }

    std::size_t length = ia.size();
    assert(ja.size() == length);
    assert(ar.size() == length);

    glp_load_matrix(prob, length - 1, &ia[0], &ja[0], &ar[0]);

    if (filename) {
      glp_write_lp(prob, 0, filename);
    }

    glp_smcp smcp;
    glp_init_smcp(&smcp);
    smcp.presolve = GLP_OFF;
    smcp.tm_lim = 2*60*1000; // 2 min
    smcp.msg_lev = GLP_MSG_ALL;

    int ret = glp_simplex(prob, &smcp);

    std::vector<double> pi(size);

    if (ret != 0 || glp_get_status(prob) != GLP_OPT) {
      glp_delete_prob(prob);

      std::cerr << "Can not solve!\n";
      return pi;
    }

    for (std::size_t k = 0; k < size; ++k) {
      pi[k] = glp_get_col_prim(prob, k + 1);
    }

    glp_delete_prob(prob);

    return pi;
  }

}


