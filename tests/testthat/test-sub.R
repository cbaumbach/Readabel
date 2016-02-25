source("setup.R")

context("[")

test_that("x[, \"snp\", drop = FALSE]", {
    expected <- data.frame(snp = x$snp, stringsAsFactors = FALSE)
    actual <- x[, "snp", drop = FALSE]
    expect_equal(expected, actual)
})

test_that("x[, c(\"snp\", \"beta_snp\"), drop = FALSE]", {
    expected <- data.frame(snp = x$snp, beta_snp = x$beta_snp, stringsAsFactors = FALSE)
    actual <- x[, c("snp", "beta_snp"), drop = FALSE]
    expect_equal(expected, actual)
})

test_that("x[]", {
    expected <- data.frame(lapply(names(x), function(name) x[[name]]), stringsAsFactors = FALSE)
    names(expected) <- names(x)
    actual <- x[]
    expect_equal(expected, actual)
})

test_that("x[,]", {
    expect_equal(x[], x[,])
})
