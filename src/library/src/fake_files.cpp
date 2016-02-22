#include "Readabel/fake_files.h"
#include <stdio.h>
#include <string.h>

#define NELEMS(x) (sizeof(x) / sizeof((x)[0]))

static const int magic_number = 6;
static const int bytes_per_double = sizeof(double);
static const int number_of_variables = 3;  // intercept, snp, sex
static const int number_of_snps = 9;
static const int number_of_traits = 7;
static const int snps_per_tile = 4;
static const int traits_per_tile = 3;
static const int bytes_per_label = 20;

void Readabel::create_fake_layout_file(std::string filename)
{
    int buffer[8];
    buffer[0] = magic_number;
    buffer[1] = bytes_per_double;
    buffer[2] = number_of_variables;
    buffer[3] = number_of_snps;
    buffer[4] = number_of_traits;
    buffer[5] = snps_per_tile;
    buffer[6] = traits_per_tile;
    buffer[7] = bytes_per_label;

    char labels[][bytes_per_label] = {
        "beta_intercept", "beta_sex", "beta_snp",                                // betas
        "se_intercept", "se_sex", "se_snp",                                      // standard errors
        "cov_sex_sex", "cov_sex_snp", "cov_snp_snp",                             // covariances
        "snp1", "snp2", "snp3", "snp4", "snp5", "snp6", "snp7", "snp8", "snp9",  // snps
        "trait1", "trait2", "trait3", "trait4", "trait5", "trait6", "trait7",    // traits
    };

    FILE *fp;
    if ((fp = fopen(filename.c_str(), "wb")) == NULL)
        return;

    fwrite((void *) buffer, sizeof(buffer), 1, fp);
    for (int i = 0; i < (int) NELEMS(labels); i++)
        fwrite((void *) labels[i], bytes_per_label, 1, fp);

    fclose(fp);
}
