#ifndef GENE_H
#define GENE_H

#include <stdlib.h>

#define MINCHAR 32
#define MAXCHAR 127

/**
 * @brief Defines a gene as a character.
 * A gene is a unit of heredity that is transferred from a parent to offspring and
 * is held to determine some characteristic of the offspring.
*/
typedef char Gene;

Gene create_gene();

#endif