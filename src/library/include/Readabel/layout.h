#ifndef LAYOUT_H
#define LAYOUT_H

#include <vector>
#include <string>

namespace Readabel {
    class Layout {
    public:
        Layout(std::string& filename);
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
    private:
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
        // Tile related.
        int number_of_tile_columns_;
        int number_of_tile_rows_;
        int number_of_tiles_;
    };
}

#endif
