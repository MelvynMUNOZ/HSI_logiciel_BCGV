import os
import subprocess
import sys

# Function that installs packages
def install_packages():
    try:
        import pandas
        import openpyxl
    except ImportError:
        subprocess.check_call([sys.executable, '-m', 'pip', 'install', '-r', 'requirements.txt'])

# Check and install packages if needed
install_packages()

import pandas as pd
import re

# A function that generates global variables
def create_global_variables(domaine, nom):
    match = re.search(r'\[(\d+); (\d+)\]', str(domaine))
    if match:
        min_val, max_val = match.groups()
        var_name = nom.upper().replace("_T", "")
        return f"#define {var_name}_MIN {min_val}\n#define {var_name}_MAX {max_val}\n\n"
    return ""

# [typedefs.h] - A function that generates types definitons
def generate_typedefs(data_df):
    header = """/**
 * \\file typedefs.h
 * \\brief Type definitions for project
 * \\details Contains all custom types and enumerations used in the project
 * \\author Raphael CAUSSE - Melvyn MUNOZ - Roland Cédric TAYO
 */

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <stdint.h>
#include <stdbool.h>

"""
    global domain_values
    domain_values = ''.join(data_df.apply(lambda row: create_global_variables(row['Domaine'], row['Nom']), axis=1))
    typedefs = header + f"// [Domain values]\n{domain_values}// Command\ntypedef bool cmd_t;\n"
    
    for _, row in data_df.iterrows():
        commentaire, genre, declaration, nom = row['Commentaire'], row['Genre'].lower(), row['Declaration'], row['Nom']
        typedefs += f"\n// {commentaire}\n"
        if genre == 'atom':
            typedefs += f"typedef {declaration} {nom};\n"
        elif genre == 'enum':
            enum_values = ',\n    '.join(map(str.strip, declaration.split(',')))
            typedefs += f"typedef enum {{\n    {enum_values},\n}} {nom};\n"
    typedefs += "\n#endif // TYPEDEFS_H"
    
    with open(os.path.join(output_dir, 'typedefs.h'), 'w') as file:
        file.write(typedefs)

# [context.h] - A function that generates context.h
def generate_context_h(donnees_df):
    context_h = """/**
 * \\file context.h
 * \\brief Context initialization and accessors for project
 * \\details Contains initialization and accessor functions for all custom types and enumerations used in the project
 * \\author Raphael CAUSSE - Melvyn MUNOZ - Roland Cédric TAYO
 */

#ifndef CONTEXT_H
#define CONTEXT_H

#include "typedefs.h"

void context_init();
"""
    for _, row in donnees_df.iterrows():
        type_name, type_def = row['Nom'], row['Type']
        context_h += f"""\n/**
 * \\brief Gets the {type_name.lower()} value.
 * \\details Returns the current state of the {type_name.lower()}.
 * \\return {type_def} : The {type_name.lower()} value.
 */
{type_def} get_{type_name.lower()}();

/**
 * \\brief Sets the {type_name.lower()} value.
 * \\details Sets the {type_name.lower()} to the given value.
 * \\param value : The new value for the {type_name.lower()}.
 */
void set_{type_name.lower()}({type_def} value);
"""
    context_h += "\n#endif // CONTEXT_H"
    
    with open(os.path.join(output_dir, 'context.h'), 'w') as file:
        file.write(context_h)

# [context.c] - A function that generates context.c
def generate_context_c(donnees_df, domain_values):
    context_c = """/**
 * \\file context.c
 * \\brief Context initialization and definitions for project
 * \\details Contains initialization and definition functions for all custom types and enumerations used in the project
 * \\author Raphael CAUSSE - Melvyn MUNOZ - Roland Cédric TAYO
 */

#include "context.h"

// Context structure
typedef struct {
"""
    for _, row in donnees_df.iterrows():
        context_c += f"    {row['Type']} {row['Nom']}; // {row['Commentaire']}\n"
    
    context_c += """} context_t;

// Global context structure instance
static context_t context;

void context_init() {
"""
    for _, row in donnees_df.iterrows():
        context_c += f"    context.{row['Nom']} = {row['Valeur d\'init']};\n"
    context_c += "}\n\n// Getters and Setters"
    
    # Ajout des getters et setters avec test de domaine si nécessaire
    for _, row in donnees_df.iterrows():
        type_name, type_def = row['Nom'], row['Type']
        var_name = type_name.upper().replace("_T", "")
        context_c += f"""
{type_def} get_{type_name.lower()}() {{
    return context.{type_name.lower()};
}}

void set_{type_name.lower()}({type_def} value) {{
"""
        if f"#define {var_name}_MIN" in domain_values and f"#define {var_name}_MAX" in domain_values:
            context_c += f"    if (value >= {var_name}_MIN && value <= {var_name}_MAX) {{\n"
            context_c += f"        context.{type_name.lower()} = value;\n    }}\n"
        else:
            context_c += f"    context.{type_name.lower()} = value;\n"
        context_c += "}\n"

    with open(os.path.join(output_dir, 'context.c'), 'w') as file:
        file.write(context_c)

# Read the excel file
file_path = 'app_struct.xlsx'
df = pd.read_excel(file_path, sheet_name=0)

# Extract "TYPES" & "DONNEES" data
types_start = df.apply(lambda row: 'TYPES' in row.values, axis=1).idxmax()
donnees_start = df.apply(lambda row: 'DONNEES' in row.values, axis=1).idxmax()
types_df = df.iloc[types_start+2 : donnees_start].dropna(how='all').reset_index(drop=True)
donnees_df = df.iloc[donnees_start+2 :].dropna(how='all').reset_index(drop=True)

# Dataframes column naming
types_df.columns = df.iloc[types_start+1]
donnees_df.columns = df.iloc[donnees_start+1]

# Output directory
output_dir = '../app/src'
os.makedirs(output_dir, exist_ok=True)

# Générate files
generate_typedefs(types_df)
generate_context_h(donnees_df)
generate_context_c(donnees_df, domain_values)

print(f"typedefs.h, context.h and context.c generated in {output_dir}.")
