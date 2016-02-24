#include "catch.hpp"
#include "Readabel/fake_files.h"
#include "Readabel/layout.h"
#include <vector>
#include <string>

using namespace Readabel;

TEST_CASE("Initialize a Layout object from a layout file and a data file", "[Layout]") {
    // =========  LAYOUT OF FAKE DATA FILE  =========
    //
    //                        snps
    //
    //              0  1  2  3    4  5  6  7    8
    //           +--------------------------------+
    //         0 |  0  1  2  3 | 12 13 14 15 | 24 |
    //         1 |  4  5  6  7 | 16 17 18 19 | 25 |
    //         2 |  8  9 10 11 | 20 21 22 23 | 26 |
    //           |-------------+-------------+----|
    // traits  3 | 27 28 29 30 | 39 40 41 42 | 51 |
    //         4 | 31 32 33 34 | 43 44 45 46 | 52 |
    //         5 | 35 36 37 38 | 47 48 49 50 | 53 |
    //           |-------------+-------------+----|
    //         6 | 54 55 56 57 | 58 59 60 61 | 62 |
    //           +--------------------------------+

    std::string fake_file_stem("build/fake_file");
    std::string fake_layout_file(fake_file_stem + ".iout");
    std::string fake_data_file(fake_file_stem + ".out");
    create_fake_layout_file(fake_layout_file);
    create_fake_data_file(fake_data_file);
    Layout layout(fake_layout_file, fake_data_file);

    SECTION("layout parameters") {
        REQUIRE(layout.magic_number() == 6);
        REQUIRE(layout.bytes_per_double() == 8);
        REQUIRE(layout.number_of_variables() == 3);
        REQUIRE(layout.number_of_snps() == 9);
        REQUIRE(layout.number_of_traits() == 7);
        REQUIRE(layout.snps_per_tile() == 4);
        REQUIRE(layout.traits_per_tile() == 3);
        REQUIRE(layout.bytes_per_label() == 20);
    }
    SECTION("beta_labels") {
        const std::vector<std::string>& beta_labels = layout.beta_labels();
        REQUIRE(beta_labels[0] == "beta_intercept");
        REQUIRE(beta_labels[1] == "beta_sex");
        REQUIRE(beta_labels[2] == "beta_snp");
    }
    SECTION("se_labels") {
        const std::vector<std::string>& se_labels = layout.se_labels();
        REQUIRE(se_labels[0] == "se_intercept");
        REQUIRE(se_labels[1] == "se_sex");
        REQUIRE(se_labels[2] == "se_snp");
    }
    SECTION("cov_labels") {
        const std::vector<std::string>& cov_labels = layout.cov_labels();
        REQUIRE(cov_labels[0] == "cov_sex_sex");
        REQUIRE(cov_labels[1] == "cov_sex_snp");
        REQUIRE(cov_labels[2] == "cov_snp_snp");
    }
    SECTION("snp_labels") {
        const std::vector<std::string>& snp_labels = layout.snp_labels();
        REQUIRE(snp_labels[0] == "snp1");
        REQUIRE(snp_labels[1] == "snp2");
        REQUIRE(snp_labels[2] == "snp3");
        REQUIRE(snp_labels[3] == "snp4");
        REQUIRE(snp_labels[4] == "snp5");
        REQUIRE(snp_labels[5] == "snp6");
        REQUIRE(snp_labels[6] == "snp7");
        REQUIRE(snp_labels[7] == "snp8");
        REQUIRE(snp_labels[8] == "snp9");
    }
    SECTION("trait_labels") {
        const std::vector<std::string>& trait_labels = layout.trait_labels();
        REQUIRE(trait_labels[0] == "trait1");
        REQUIRE(trait_labels[1] == "trait2");
        REQUIRE(trait_labels[2] == "trait3");
        REQUIRE(trait_labels[3] == "trait4");
        REQUIRE(trait_labels[4] == "trait5");
        REQUIRE(trait_labels[5] == "trait6");
        REQUIRE(trait_labels[6] == "trait7");
    }
    SECTION("number_of_tiles") {
        REQUIRE(layout.number_of_tiles() == 9);
    }
    SECTION("number_of_cells") {
        const int size[] = {
            //   snps
            4*3, 4*3, 1*3,
            4*3, 4*3, 1*3,  // traits
            4*1, 4*1, 1*1,
        };
        for (int tile = 0; tile < layout.number_of_tiles(); tile++)
            REQUIRE(layout.number_of_cells(tile) == size[tile]);
    }
    SECTION("find_snp_in_cell") {
        const int index_of_snp_in_cell[] = {
            0, 1, 2, 3,  0, 1, 2, 3,  0, 1, 2, 3,  // tile 1
            4, 5, 6, 7,  4, 5, 6, 7,  4, 5, 6, 7,  // tile 2
            8,           8,           8,           // tile 3
            0, 1, 2, 3,  0, 1, 2, 3,  0, 1, 2, 3,  // tile 4
            4, 5, 6, 7,  4, 5, 6, 7,  4, 5, 6, 7,  // tile 5
            8,           8,           8,           // tile 6
            0, 1, 2, 3,                            // tile 7
            4, 5, 6, 7,                            // tile 8
            8,                                     // tile 9
        };
        const std::vector<std::string>& snps = layout.snp_labels();

        int number_of_cells = layout.number_of_snps() * layout.number_of_traits();
        for (int cell = 0; cell < number_of_cells; cell++)
            REQUIRE(layout.find_snp_in_cell(cell) == snps[index_of_snp_in_cell[cell]]);
    }
    SECTION("find_trait_in_cell") {
        const int index_of_trait_in_cell[] = {
            0, 0, 0, 0,  1, 1, 1, 1,  2, 2, 2, 2,  // tile 1
            0, 0, 0, 0,  1, 1, 1, 1,  2, 2, 2, 2,  // tile 2
            0,           1,           2,           // tile 2
            3, 3, 3, 3,  4, 4, 4, 4,  5, 5, 5, 5,  // tile 3
            3, 3, 3, 3,  4, 4, 4, 4,  5, 5, 5, 5,  // tile 4
            3,           4,           5,           // tile 5
            6, 6, 6, 6,                            // tile 6
            6, 6, 6, 6,                            // tile 7
            6,                                     // tile 8
        };
        const std::vector<std::string>& traits = layout.trait_labels();

        int number_of_cells = layout.number_of_snps() * layout.number_of_traits();
        for (int cell = 0; cell < number_of_cells; cell++)
            REQUIRE(layout.find_trait_in_cell(cell) == traits[index_of_trait_in_cell[cell]]);
    }
}
