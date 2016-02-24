# =========  LAYOUT OF FAKE DATA FILE  =========
#
#                        snps
#
#              0  1  2  3    4  5  6  7    8
#           +--------------------------------+
#         0 |  0  1  2  3 | 12 13 14 15 | 24 |
#         1 |  4  5  6  7 | 16 17 18 19 | 25 |
#         2 |  8  9 10 11 | 20 21 22 23 | 26 |
#           |-------------+-------------+----|
# traits  3 | 27 28 29 30 | 39 40 41 42 | 51 |
#         4 | 31 32 33 34 | 43 44 45 46 | 52 |
#         5 | 35 36 37 38 | 47 48 49 50 | 53 |
#           |-------------+-------------+----|
#         6 | 54 55 56 57 | 58 59 60 61 | 62 |
#           +--------------------------------+

fake_file_stem <- file.path(tempdir(), "fake_file")
fake_layout_file <- paste0(fake_file_stem, ".iout")
fake_data_file <- paste0(fake_file_stem, ".out")
create_fake_layout_file(fake_layout_file)
create_fake_data_file(fake_data_file)
x <- read_omicabel(fake_layout_file, fake_data_file)

snp_labels <- paste0("snp", 1:9)
trait_labels <- paste0("trait", 1:7)
covariates <- c("intercept", "sex", "snp")
beta_labels <- paste0("beta_", covariates)
se_labels <- paste0("se_", covariates)
cov_labels <- paste0("cov_", c("sex_sex", "sex_snp", "snp_snp"))

trait_column <- c(
    # ============== ROW 1 =================
    # column 1
    "trait1", "trait1", "trait1", "trait1",
    "trait2", "trait2", "trait2", "trait2",
    "trait3", "trait3", "trait3", "trait3",
    # column 2
    "trait1", "trait1", "trait1", "trait1",
    "trait2", "trait2", "trait2", "trait2",
    "trait3", "trait3", "trait3", "trait3",
    # column 3
    "trait1",
    "trait2",
    "trait3",

    # ============== ROW 2 =================
    # column 1
    "trait4", "trait4", "trait4", "trait4",
    "trait5", "trait5", "trait5", "trait5",
    "trait6", "trait6", "trait6", "trait6",
    # column 2
    "trait4", "trait4", "trait4", "trait4",
    "trait5", "trait5", "trait5", "trait5",
    "trait6", "trait6", "trait6", "trait6",
    # column 3
    "trait4",
    "trait5",
    "trait6",

    # ============== ROW 3 =================
    # column 1
    "trait7", "trait7", "trait7", "trait7",
    # column 2
    "trait7", "trait7", "trait7", "trait7",
    # column 3
    "trait7")

snp_column <- c(
    # ============== ROW 1 =================
    # column 1
    "snp1", "snp2", "snp3", "snp4",
    "snp1", "snp2", "snp3", "snp4",
    "snp1", "snp2", "snp3", "snp4",
    # column 2
    "snp5", "snp6", "snp7", "snp8",
    "snp5", "snp6", "snp7", "snp8",
    "snp5", "snp6", "snp7", "snp8",
    # column 3
    "snp9",
    "snp9",
    "snp9",

    # ============== ROW 2 =================
    # column 1
    "snp1", "snp2", "snp3", "snp4",
    "snp1", "snp2", "snp3", "snp4",
    "snp1", "snp2", "snp3", "snp4",
    # column 2
    "snp5", "snp6", "snp7", "snp8",
    "snp5", "snp6", "snp7", "snp8",
    "snp5", "snp6", "snp7", "snp8",
    # column 3
    "snp9",
    "snp9",
    "snp9",

    # ============== ROW 3 =================
    # column 1
    "snp1", "snp2", "snp3", "snp4",
    # column 2
    "snp5", "snp6", "snp7", "snp8",
    # column 3
    "snp9")
