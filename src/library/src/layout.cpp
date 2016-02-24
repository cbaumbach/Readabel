#include "Readabel/layout.h"
#include <stdio.h>
#include <stddef.h>
#include <algorithm>

using namespace Readabel;

static void read_labels(std::vector<std::string>& labels, int bytes_per_label, int number_of_labels, FILE *fp);

Layout::Layout(const std::string& layout_file, const std::string& data_file)
    : layout_file_(layout_file), data_file_(data_file)
{
    FILE *fp;
    if ((fp = fopen(layout_file_.c_str(), "rb")) == NULL)
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
    std::copy(beta_labels_.begin(), beta_labels_.end(), std::back_inserter(column_labels_));
    std::copy(se_labels_.begin(), se_labels_.end(), std::back_inserter(column_labels_));
    std::copy(cov_labels_.begin(), cov_labels_.end(), std::back_inserter(column_labels_));

    fclose(fp);

    number_of_tile_columns_ = (number_of_snps_ - 1) / snps_per_tile_ + 1;
    number_of_tile_rows_ = (number_of_traits_ - 1) / traits_per_tile_ + 1;
    number_of_tiles_ = number_of_tile_columns_ * number_of_tile_rows_;
    int snps_in_last_tile_column = number_of_snps_ % snps_per_tile_;
    int traits_in_last_tile_row = number_of_traits_ % traits_per_tile_;

    int size[2][2];
    size[0][0] = snps_per_tile_ * traits_per_tile_;
    size[0][1] = snps_per_tile_ * traits_in_last_tile_row;
    size[1][0] = snps_in_last_tile_column * traits_per_tile_;
    size[1][1] = snps_in_last_tile_column * traits_in_last_tile_row;

    for (int tile = 0; tile < number_of_tiles_; tile++)
        number_of_cells_.push_back(size
            [is_in_last_tile_column(tile)]
            [is_in_last_tile_row(tile)]);

    number_of_doubles_per_cell_ = number_of_variables_ + number_of_variables_ + number_of_covariances;
    max_number_of_cells_per_tile_ = snps_per_tile_ * traits_per_tile_;
    cell_buffer_ = new double[number_of_doubles_per_cell_ * max_number_of_cells_per_tile_];
}

Layout::~Layout()
{
    delete[] cell_buffer_;
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
    delete[] buffer;
}

bool Layout::is_in_last_tile_column(int tile) const
{
    return (tile + 1) % number_of_tile_columns_ == 0;
}

bool Layout::is_in_last_tile_row(int tile) const
{
    return tile >= (number_of_tile_rows_ - 1) * number_of_tile_columns_;
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

int Layout::number_of_cells(int tile) const
{
    return number_of_cells_[tile];;
}

std::vector<double>* Layout::column(const std::string& name) const
{
    FILE *fp;
    if ((fp = fopen(data_file_.c_str(), "rb")) == NULL)
        return NULL;
    std::vector<double>* column = new std::vector<double>;
    int column_offset = std::find(column_labels_.begin(), column_labels_.end(), name) - column_labels_.begin();
    for (int tile = 0; tile < number_of_tiles_; tile++) {
        int number_of_bytes = number_of_cells(tile) * number_of_doubles_per_cell_ * sizeof(double);
        fread((void *) cell_buffer_, number_of_bytes, 1, fp);
        for (int cell = 0; cell < number_of_cells(tile); cell++)
            column->push_back(cell_buffer_[cell * number_of_doubles_per_cell_ + column_offset]);
    }
    fclose(fp);

    return column;
}

std::vector<std::string>* Layout::snp_column()
{
    std::vector<std::string>* column = new std::vector<std::string>;
    int number_of_cells = number_of_snps_ * number_of_traits_;
    for (int cell = 0; cell < number_of_cells; cell++)
        column->push_back(find_snp_in_cell(cell));

    return column;
}

std::vector<std::string>* Layout::trait_column()
{
    std::vector<std::string>* column = new std::vector<std::string>;
    int number_of_cells = number_of_snps_ * number_of_traits_;
    for (int cell = 0; cell < number_of_cells; cell++)
        column->push_back(find_trait_in_cell(cell));

    return column;
}

const std::string& Layout::find_snp_in_cell(int cell)
{
    find_in_cell(cell);
    return *snp_in_cell_;
}

const std::string& Layout::find_trait_in_cell(int cell)
{
    find_in_cell(cell);
    return *trait_in_cell_;
}

void Layout::find_in_cell(int cell)
{
    int x = cell;

    // Find tile row.
    int cells_per_tile_row = number_of_snps_ * traits_per_tile_;
    int tile_row = x / cells_per_tile_row;
    x %= cells_per_tile_row;

    // Find tile column.
    int cells_per_tile;
    if (traits_per_tile_ <= number_of_traits_ - tile_row * traits_per_tile_)
        cells_per_tile = snps_per_tile_ * traits_per_tile_;
    else
        cells_per_tile = snps_per_tile_
            * (number_of_traits_ - tile_row * traits_per_tile_);
    int tile_col = x / cells_per_tile;
    x %= cells_per_tile;

    // Find row within tile.
    int snps_in_this_tile;
    if (snps_per_tile_ <= number_of_snps_ - tile_col * snps_per_tile_)
        snps_in_this_tile = snps_per_tile_;
    else
        snps_in_this_tile = number_of_snps_ - tile_col * snps_per_tile_;
    int row_within_tile = x / snps_in_this_tile;
    x %= snps_in_this_tile;

    // Find column within tile.
    int column_within_tile = x;

    snp_in_cell_ = &snp_labels_[tile_col * snps_per_tile_ + column_within_tile];
    trait_in_cell_ = &trait_labels_[tile_row * traits_per_tile_ + row_within_tile];
}
