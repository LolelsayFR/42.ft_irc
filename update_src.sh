#!/bin/bash

SRC_MK="Sources.mk"
OS=$(uname)
macOS=false
[[ $OS == "Darwin" ]] && macOS=true

update_sources() {
    local src_dir=$1
    local var_name=$2

    # Vérifier si le répertoire source existe
    if [[ ! -d "$src_dir" ]]; then
        printf "\033[0;91mErreur : le répertoire source %s n'existe pas.\n\033[0;39m" "$src_dir"
        exit 1
    fi

    # Trouver les fichiers sources
    if [[ $macOS == true ]]; then
        src_files=$(find "$src_dir" -type f \( -name "*.cpp" -o -name "*.m" \))
    else
        src_files=$(find "$src_dir" -type f -name "*.cpp")
    fi

    # Vérifier s'il y a des fichiers sources
    if [[ -z "$src_files" ]]; then
        printf "\033[0;91mAucun fichier source trouvé dans %s.\n\033[0;39m" "$src_dir"
        exit 1
    fi

    # Construire la nouvelle ligne pour la variable
    new_line="$var_name \\"
    while IFS= read -r file; do
        new_line+="$file "
    done <<< "$src_files"


    # Vérifier si le fichier Sources.mk existe
    if [[ -f "$SRC_MK" ]]; then
        # Remplacer la ligne correspondante
        if [[ $macOS == true ]]; then
            sed -i "" "/^$var_name/c\\
$new_line" "$SRC_MK"
        else
            sed -i "/^$var_name/c\\
$new_line" "$SRC_MK"
        fi
        printf "\033[0;92mLe fichier %s a été mis à jour pour %s.\n\033[0;39m" "$SRC_MK" "$var_name"
    else
        printf "\033[0;91mErreur : le fichier %s n'existe pas.\n\033[0;39m" "$SRC_MK"
        exit 1
    fi
}



# Appels à la fonction pour mettre à jour plusieurs variables
update_sources "./src" "SRC_MAIN :="


