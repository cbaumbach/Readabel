source("setup.R")

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
