#' Proxy of a binary OmicABEL output file.
#'
#' @slot pointer External pointer to a C++ object.
setClass("Readabel",
    slots = list(pointer = "externalptr"))

setGeneric("names")

#' Return column names.
#'
#' @param x An object of class Readabel.
#' @export
setMethod("names", "Readabel", function(x) {
    .Call("Layout__names", x@pointer)
})

#' Return a new Readabel object.
#'
#' @param x An uninitialized object of class Readabel
#' @param filename The path to a binary OmicABEL output file
setMethod("initialize", "Readabel", function(.Object, filename) {
    .Object@pointer <- .Call("Layout__new", filename)
    .Object
})

#' Read a binary OmicABEL file.
#'
#' @param filename The path to the file.
#' @export
read_omicabel <- function(filename) {
    new("Readabel", filename)
}

setGeneric("nrow")

#' Return number of rows.
#'
#' @param x An object of class Readabel
#' @export
setMethod("nrow", "Readabel", function(x) {
    .Call("Layout__nrow", x@pointer)
})

setGeneric("ncol")

#' Return number of columns.
#'
#' @param x An object of class Readabel
#' @export
setMethod("ncol", "Readabel", function(x) {
    length(names(x))
})

setGeneric("dim")

#' Return dimensions
#'
#' @param x An object of class Readabel
#' @export
setMethod("dim", "Readabel", function(x) {
    c(nrow(x), ncol(x))
})
