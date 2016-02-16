fake_layout_file <- file.path(tempdir(), "fake_layout_file.iout")
create_fake_layout_file(fake_layout_file)
x <- read_omicabel(fake_layout_file)

snp_labels <- paste0("snp", 1:9)
trait_labels <- paste0("trait", 1:7)
covariates <- c("intercept", "sex", "snp")
beta_labels <- paste0("beta_", covariates)
se_labels <- paste0("se_", covariates)
cov_labels <- paste0("cov_", c("sex_sex", "sex_snp", "snp_snp"))

test_that("names", {
    expect_equal(names(x), c("trait", "snp", beta_labels, se_labels, cov_labels))
})

test_that("nrow", {
    expect_equal(nrow(x), length(snp_labels) * length(trait_labels))
})

test_that("ncol", {
    expect_equal(ncol(x), length(names(x)))
})

test_that("dim", {
    expect_equal(dim(x), c(nrow(x), ncol(x)))
})

test_that("snpNames", {
    expect_equal(snpNames(x), snp_labels)
})

test_that("traitNames", {
    expect_equal(traitNames(x), trait_labels)
})
