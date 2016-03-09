#include <Rcpp.h>
#include <vector>
#include <string>
#include "Readabel/layout.h"

RcppExport SEXP rcpp_new(SEXP layout_file, SEXP data_file)
{
    Rcpp::XPtr<Readabel::Layout> ptr(new Readabel::Layout(Rcpp::as<std::string>(layout_file), Rcpp::as<std::string>(data_file)), true);

    return ptr;
}

RcppExport SEXP rcpp_names(SEXP xp)
{
    Rcpp::XPtr<Readabel::Layout> ptr(xp);
    Rcpp::CharacterVector trait_and_snp = Rcpp::CharacterVector::create("trait", "snp");
    Rcpp::CharacterVector beta(Rcpp::wrap(ptr->beta_labels()));
    Rcpp::CharacterVector se(Rcpp::wrap(ptr->se_labels()));
    Rcpp::CharacterVector cov(Rcpp::wrap(ptr->cov_labels()));
    int number_of_columns = trait_and_snp.size() + beta.size() + se.size() + cov.size();
    Rcpp::CharacterVector names(number_of_columns);

    int i = 0;
    for (int j = 0; j < trait_and_snp.size(); j++)
        names[i++] = trait_and_snp[j];
    for (int j = 0; j < beta.size(); j++)
        names[i++] = beta[j];
    for (int j = 0; j < se.size(); j++)
        names[i++] = se[j];
    for (int j = 0; j < cov.size(); j++)
        names[i++] = cov[j];

    return names;
}

RcppExport SEXP rcpp_nrow(SEXP xp)
{
    Rcpp::XPtr<Readabel::Layout> ptr(xp);
    int number_of_snps = ptr->number_of_snps();
    int number_of_traits = ptr->number_of_traits();

    return Rcpp::wrap(number_of_snps * number_of_traits);
}

RcppExport SEXP rcpp_snpNames(SEXP xp)
{
    Rcpp::XPtr<Readabel::Layout> ptr(xp);
    Rcpp::CharacterVector snp_labels(Rcpp::wrap(ptr->snp_labels()));

    return snp_labels;
}

RcppExport SEXP rcpp_traitNames(SEXP xp)
{
    Rcpp::XPtr<Readabel::Layout> ptr(xp);
    Rcpp::CharacterVector trait_labels(Rcpp::wrap(ptr->trait_labels()));

    return trait_labels;
}

RcppExport SEXP rcpp_get_numeric_column(SEXP xp, SEXP column_index)
{
    Rcpp::XPtr<Readabel::Layout> ptr(xp);
    int number_of_rows = ptr->number_of_snps() * ptr->number_of_traits();
    Rcpp::NumericVector column_(number_of_rows);
    // Adjust column index from R to C++: The leading snp and trait
    // columns are virtual; they exist only in the R world.  In the
    // data file, there are only numeric columns.  So we subtract 2.
    // In addition, columns in R are 1-based while in C++ they are
    // 0-based.  Thus we subtract one more, for a total of 3.
    int column_index_ = Rcpp::as<int>(column_index) - 3;

    ptr->column(column_index_, &column_[0]);

    return column_;
}

RcppExport SEXP rcpp_get_snp_column(SEXP xp)
{
    Rcpp::XPtr<Readabel::Layout> ptr(xp);

    return Rcpp::wrap(*ptr->snp_column());
}

RcppExport SEXP rcpp_get_trait_column(SEXP xp)
{
    Rcpp::XPtr<Readabel::Layout> ptr(xp);

    return Rcpp::wrap(*ptr->trait_column());
}
