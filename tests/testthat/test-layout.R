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

test_that("$", {
    expect_equal(nrow(x), length(x$beta_snp))
    expect_true(all(x$beta_snp %in% 1:nrow(x)))
    expect_equal(x$beta_snp, x$beta_sex)
    expect_equal(snp_column, x$snp)
    expect_equal(trait_column, x$trait)
    expect_error(x$foobar)
})

test_that("head", {
    expect_equal(head(d), head(x))
    expect_equal(head(d, 2), head(x, 2))
})

test_that("tail", {
    expect_equal(tail(d), tail(x))
    expect_equal(tail(d, 2), tail(x, 2))
})

test_that("colnames", {
    expect_equal(names(x), colnames(x))
})

test_that("rownames", {
    all_rows <- as.character(1:nrow(x))
    expect_equal(all_rows, rownames(x))
    expect_equal(all_rows[c(TRUE, FALSE)], rownames(x[c(TRUE, FALSE), ]))
})

test_that("row.names", {
    all_rows <- as.character(1:nrow(x))
    expect_equal(all_rows, row.names(x))
    expect_equal(all_rows[c(TRUE, FALSE)], row.names(x[c(TRUE, FALSE), ]))
})
