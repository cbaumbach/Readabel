#ifndef LAYOUT_H
#define LAYOUT_H

#include <vector>
#include <string>

namespace Readabel {
    class Layout {
    public:
        Layout(const std::string& layout_file, const std::string& data_file);
        ~Layout();
        int magic_number() const;
        int bytes_per_double() const;
        int number_of_variables() const;
        int number_of_snps() const;
        int number_of_traits() const;
        int snps_per_tile() const;
        int traits_per_tile() const;
        int bytes_per_label() const;
        const std::vector<std::string>& beta_labels() const;
        const std::vector<std::string>& se_labels() const;
        const std::vector<std::string>& cov_labels() const;
        const std::vector<std::string>& snp_labels() const;
        const std::vector<std::string>& trait_labels() const;
        int number_of_tiles() const;
        int number_of_cells(int tile) const;
        void columns(const std::vector<int>& column_indices, std::vector<double*>& columns, const std::vector<int>& row_indices) const;
        const std::string& find_snp_in_cell(int cell);
        const std::string& find_trait_in_cell(int cell);
        std::vector<std::string>* snp_column(const std::vector<int>& row_indices);
        std::vector<std::string>* trait_column(const std::vector<int>& row_indices);
    private:
        bool is_in_last_tile_column(int tile) const;
        bool is_in_last_tile_row(int tile) const;
        void find_in_cell(int cell);
        std::vector<std::string>* string_column(const std::string& (Layout::*find_thing_in_cell)(int), const std::vector<int>& row_indices);
        std::string layout_file_;
        std::string data_file_;
        int magic_number_;
        int bytes_per_double_;
        int number_of_variables_;
        int number_of_snps_;
        int number_of_traits_;
        int snps_per_tile_;
        int traits_per_tile_;
        int bytes_per_label_;
        std::vector<std::string> beta_labels_;
        std::vector<std::string> se_labels_;
        std::vector<std::string> cov_labels_;
        std::vector<std::string> snp_labels_;
        std::vector<std::string> trait_labels_;
        std::vector<std::string> column_labels_;
        // Tile related.
        int number_of_tile_columns_;
        int number_of_tile_rows_;
        int number_of_tiles_;
        std::vector<int> number_of_cells_;
        int number_of_doubles_per_cell_;
        int max_number_of_cells_per_tile_;
        double *cell_buffer_;
        std::string* snp_in_cell_;
        std::string* trait_in_cell_;
    };
}

#endif
