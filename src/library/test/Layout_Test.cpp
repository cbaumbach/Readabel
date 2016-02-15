#include "catch.hpp"
#include "Readabel/fake_files.h"
#include "Readabel/layout.h"
#include <vector>
#include <string>

using namespace Readabel;

typedef std::vector<std::string> vector_of_strings;

void compare_byte_by_byte(const char *s, const char *t);

TEST_CASE("Initialize a Layout object from a layout file", "[Layout]") {
    std::string fake_layout_file("build/fake_layout_file.iout");
    create_fake_layout_file(fake_layout_file);
    Layout layout(fake_layout_file);

    REQUIRE(layout.magic_number() == 6);
    REQUIRE(layout.bytes_per_double() == 8);
    REQUIRE(layout.number_of_variables() == 3);
    REQUIRE(layout.number_of_snps() == 9);
    REQUIRE(layout.number_of_traits() == 7);
    REQUIRE(layout.snps_per_tile() == 4);
    REQUIRE(layout.traits_per_tile() == 3);
    REQUIRE(layout.bytes_per_label() == 20);

    const vector_of_strings& beta_labels = layout.beta_labels();
    REQUIRE(beta_labels[0] == "beta_intercept");
    REQUIRE(beta_labels[1] == "beta_sex");
    REQUIRE(beta_labels[2] == "beta_snp");

    const vector_of_strings& se_labels = layout.se_labels();
    REQUIRE(se_labels[0] == "se_intercept");
    REQUIRE(se_labels[1] == "se_sex");
    REQUIRE(se_labels[2] == "se_snp");

    const vector_of_strings& cov_labels = layout.cov_labels();
    REQUIRE(cov_labels[0] == "cov_sex_sex");
    REQUIRE(cov_labels[1] == "cov_sex_snp");
    REQUIRE(cov_labels[2] == "cov_snp_snp");

    const vector_of_strings& snp_labels = layout.snp_labels();
    REQUIRE(snp_labels[0] == "snp1");
    REQUIRE(snp_labels[1] == "snp2");
    REQUIRE(snp_labels[2] == "snp3");
    REQUIRE(snp_labels[3] == "snp4");
    REQUIRE(snp_labels[4] == "snp5");
    REQUIRE(snp_labels[5] == "snp6");
    REQUIRE(snp_labels[6] == "snp7");
    REQUIRE(snp_labels[7] == "snp8");
    REQUIRE(snp_labels[8] == "snp9");

    const vector_of_strings& trait_labels = layout.trait_labels();
    REQUIRE(trait_labels[0] == "trait1");
    REQUIRE(trait_labels[1] == "trait2");
    REQUIRE(trait_labels[2] == "trait3");
    REQUIRE(trait_labels[3] == "trait4");
    REQUIRE(trait_labels[4] == "trait5");
    REQUIRE(trait_labels[5] == "trait6");
}
