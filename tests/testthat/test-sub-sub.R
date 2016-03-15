source("setup.R")

context("[[")

test_that("columns can be accessed by name", {
    for (column in names(x)) {
        clear_cache(x)
        expect_equal(d[[column]], x[[column]])
    }
})

test_that("columns can be accessed by index", {
    for (column in names(x)) {
        clear_cache(x)
        expect_equal(d[[column]], x[[which(names(x) == column)]])
    }
})

test_that("numeric columns are all equal", {
    for (column in numeric_columns) {
        clear_cache(x)
        expect_equal(x[[numeric_columns[1]]], x[[column]])
    }
})

test_that("numeric columns contain all numbers from 1 to nrow(x)", {
    clear_cache(x)
    expect_true(all(x[[numeric_columns[1]]] %in% 1:nrow(x)))
})

test_that("every column contains as many values as there are rows", {
    clear_cache(x)
    expect_equal(nrow(x), length(x[[1]]))
})

test_that("invalid column indexes result in errors", {
    expect_error(x[[-1]])
    expect_error(x[[999]])
    expect_error(x[[integer()]])
})

test_that("invalid column names result in errors", {
    expect_error(x[[""]])
    expect_error(x[["foobar"]])
    expect_error(x[[character()]])
})
