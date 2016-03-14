source("setup.R")

context("cache")

test_that("caching works with [[", {
    clear_cache(x)
    column <- "snp"
    expect_false(is_in_cache(x, column))
    x[[column]]
    expect_true(is_in_cache(x, column))
})

test_that("caching works when more than one column is extracted", {
    clear_cache(x)
    columns <- c("beta_sex", "snp")
    for (col in columns)
        expect_false(is_in_cache(x, col))
    x[columns]
    for (col in columns)
        expect_true(is_in_cache(x, col))
})
