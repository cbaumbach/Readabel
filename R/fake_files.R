#' Create a fake OmicABEL layout file
#'
#' @param filename The path to the file.
#' @export
create_fake_layout_file <- function(filename) {
    invisible(.Call("create_fake_layout_file", PACKAGE = "Readabel", filename))
}

#' Create a fake OmicABEL data file
#'
#' @param filename The path to the file.
#' @export
create_fake_data_file <- function(filename) {
    invisible(.Call("create_fake_data_file", PACKAGE = "Readabel", filename))
}
