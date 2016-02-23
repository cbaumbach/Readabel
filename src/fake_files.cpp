#include <Rcpp.h>
#include "Readabel/fake_files.h"
#include <vector>
#include <string>

RcppExport SEXP create_fake_layout_file(SEXP filenames_) {
    std::vector<std::string> filenames = Rcpp::as< std::vector<std::string> >(filenames_);
    Readabel::create_fake_layout_file(filenames[0]);
    return R_NilValue;
}

RcppExport SEXP create_fake_data_file(SEXP filenames_) {
    std::vector<std::string> filenames = Rcpp::as< std::vector<std::string> >(filenames_);
    Readabel::create_fake_data_file(filenames[0]);
    return R_NilValue;
}
