fake_layout_file <- file.path(tempdir(), "fake_layout_file.iout")
create_fake_layout_file(fake_layout_file)
x <- read_omicabel(fake_layout_file)

test_that("names", {
    column_names <- c("trait", "snp",
        "beta_intercept", "beta_sex", "beta_snp",
        "se_intercept", "se_sex", "se_snp",
        "cov_sex_sex", "cov_sex_snp", "cov_snp_snp")
    expect_equal(names(x), column_names)
})

test_that("nrow", {
    number_of_snps <- 9L
    number_of_traits <- 7L
    expect_equal(nrow(x), number_of_snps * number_of_traits)
})

test_that("ncol", {
    expect_equal(ncol(x), length(names(x)))
})

test_that("dim", {
    expect_equal(dim(x), c(nrow(x), ncol(x)))
})

test_that("snpNames", {
    expect_equal(snpNames(x), paste0("snp", 1:9))
})
