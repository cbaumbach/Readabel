#include "Readabel/fake_files.h"
#include <stdio.h>
#include <string.h>

#define NELEMS(x) (sizeof(x) / sizeof((x)[0]))

static const int BYTES_PER_LABEL = 20;

void Readabel::create_fake_layout_file(std::string filename)
{
    FILE *fp;
    int i, buffer[] = {
        6,  // magic number
        8,  // bytes per double
        3,  // number of variables: intercept, snp, sex
        9,  // number of snps
        7,  // number of traits
        4,  // snps per tile
        3,  // traits per tile
        BYTES_PER_LABEL
    };
    char labels[][BYTES_PER_LABEL] = {
        "beta_intercept", "beta_sex", "beta_snp",                                // betas
        "se_intercept", "se_sex", "se_snp",                                      // standard errors
        "cov_sex_sex", "cov_sex_snp", "cov_snp_snp",                             // covariances
        "snp1", "snp2", "snp3", "snp4", "snp5", "snp6", "snp7", "snp8", "snp9",  // snps
        "trait1", "trait2", "trait3", "trait4", "trait5", "trait6", "trait7",    // traits
    };

    if ((fp = fopen(filename.c_str(), "wb")) == NULL)
        return;

    fwrite((void *) buffer, sizeof(buffer), 1, fp);
    for (i = 0; i < (int) NELEMS(labels); i++)
        fwrite((void *) labels[i], BYTES_PER_LABEL, 1, fp);

    fclose(fp);
}
