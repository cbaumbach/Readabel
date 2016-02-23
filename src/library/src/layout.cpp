#include "Readabel/layout.h"
#include <stdio.h>
#include <stddef.h>

using namespace Readabel;

static void read_labels(std::vector<std::string>& labels, int bytes_per_label, int number_of_labels, FILE *fp);

Layout::Layout(std::string& filename)
{
    FILE *fp;
    if ((fp = fopen(filename.c_str(), "rb")) == NULL)
        return;

    int numbers[8];
    fread((void *) numbers, sizeof(numbers), 1, fp);
    magic_number_        = numbers[0];
    bytes_per_double_    = numbers[1];
    number_of_variables_ = numbers[2];
    number_of_snps_      = numbers[3];
    number_of_traits_    = numbers[4];
    snps_per_tile_       = numbers[5];
    traits_per_tile_     = numbers[6];
    bytes_per_label_     = numbers[7];

    read_labels(beta_labels_, bytes_per_label_, number_of_variables_, fp);
    read_labels(se_labels_, bytes_per_label_, number_of_variables_, fp);
    int number_of_covariances = ((number_of_variables_ - 1) * number_of_variables_) / 2;
    read_labels(cov_labels_, bytes_per_label_, number_of_covariances, fp);
    read_labels(snp_labels_, bytes_per_label_, number_of_snps_, fp);
    read_labels(trait_labels_, bytes_per_label_, number_of_traits_, fp);

    fclose(fp);

    number_of_tile_columns_ = (number_of_snps_ - 1) / snps_per_tile_ + 1;
    number_of_tile_rows_ = (number_of_traits_ - 1) / traits_per_tile_ + 1;
    number_of_tiles_ = number_of_tile_columns_ * number_of_tile_rows_;
}

static void read_labels(std::vector<std::string>& labels, int bytes_per_label, int number_of_labels, FILE *fp)
{
    int number_of_bytes_to_read = bytes_per_label * number_of_labels;
    char *buffer = new char[number_of_bytes_to_read];
    fread((void *) buffer, bytes_per_label, number_of_labels, fp);
    for (int i = 0; i < number_of_labels; i++) {
        const char *start = buffer + i * bytes_per_label;
        labels.push_back(std::string(start));
    }
    delete buffer;
}

int Layout::magic_number() const
{
    return magic_number_;
}

int Layout::bytes_per_double() const
{
    return bytes_per_double_;
}

int Layout::number_of_variables() const
{
    return number_of_variables_;
}

int Layout::number_of_snps() const
{
    return number_of_snps_;
}

int Layout::number_of_traits() const
{
    return number_of_traits_;
}

int Layout::snps_per_tile() const
{
    return snps_per_tile_;
}

int Layout::traits_per_tile() const
{
    return traits_per_tile_;
}

int Layout::bytes_per_label() const
{
    return bytes_per_label_;
}

const std::vector<std::string>& Layout::beta_labels() const
{
    return beta_labels_;
}

const std::vector<std::string>& Layout::se_labels() const
{
    return se_labels_;
}

const std::vector<std::string>& Layout::cov_labels() const
{
    return cov_labels_;
}

const std::vector<std::string>& Layout::snp_labels() const
{
    return snp_labels_;
}

const std::vector<std::string>& Layout::trait_labels() const
{
    return trait_labels_;
}

int Layout::number_of_tiles() const
{
    return number_of_tiles_;
}
