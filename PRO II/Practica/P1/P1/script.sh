#!/bin/bash


#para dar formato
ROJO=`tput setaf 1`
VERDE=`tput setaf 2`
AMARILLO=`tput setaf 3`
RESET=`tput sgr0`
SUBR=`tput smul`


#para cálculos resultados
totalLineas=0
lineasDiferentes=0
porcentajeLineas=0
MAIN_NAME=main

usage() { echo "Usage: $0 [-p <main|test>] [-v]" 1>&2; exit 1; }

# Comprobar parametros de entrada
VERBOSE=false
while getopts "p:v" opt; do
    case "$opt" in
      p)  MAIN_NAME=$OPTARG
          if [[ "$MAIN_NAME" != "main" && "$MAIN_NAME" != "test" ]]; then
            usage
          fi
          ;;
      v)
          VERBOSE=true
          echo VERBOSETRUE
          ;;
      *)
          usage
          ;;
    esac
done
MAIN="./"$MAIN_NAME
if [ "$MAIN_NAME" = "main" ]
then
  ficherosEntrada=("script_minimos/new" "script_minimos/vote" "script_minimos/disqualify1" "script_minimos/disqualify2")
  ficherosRef=("script_minimos/new_ref" "script_minimos/vote_ref" "script_minimos/disqualify1_ref" "script_minimos/disqualify2_ref")
else
  ficherosEntrada=("")
  ficherosRef=("script_test/ref")
fi

# Funcion para comprobar la salida del programa
# Parametros de entrada:
#   - descripcion tipo de lista
#   - nombre unit a probar
#   - salida verbosa
# Parametros de salida:
#   - Exito (1) o fallo (0) en la comprobacion
function check_output {
    LIST_TYPE_DESC=$1
    DEPS=$2
    VERBOSE=$3

    rm -f ${MAIN}

    printf "${AMARILLO}Running script for ${LIST_TYPE_DESC} list...${RESET}"
    printf "\n${AMARILLO}Compiling "$MAIN_NAME" program using ${LIST_TYPE_DESC} list with\ngcc -Wall -Wextra ${DEPS} ${MAIN}.c -o ${MAIN}${RESET}\n"

    gcc -Wall -Wextra ${DEPS} ${MAIN}.c -o ${MAIN}

    if [ -f ${MAIN} ]
    then
        allOK=1
        printf "\n${AMARILLO}Checking "$MAIN_NAME" program output using ${LIST_TYPE_DESC} list...\n${RESET}"
	    printf "\n${SUBR}Input file${RESET}                          ${SUBR}Result${RESET}  ${SUBR}Notes${RESET}\n"
      for index in ${!ficherosEntrada[*]}
	    do
        nombre=${ficherosEntrada[$index]}
        if [ "$nombre" != "" ]
        then
	    	    ficheroEntrada="$nombre".txt
        else
            ficheroEntrada=""
            nombre="script_test/test"
            #nombre=$index
        fi
	    	ficheroReferencia=${ficherosRef[$index]}.txt
	    	ficheroSalida="$nombre"_${LIST_TYPE_DESC}.txt
	    	ficheroDiff="$nombre"_diff.txt
	    	${MAIN} $ficheroEntrada > $ficheroSalida
	    	diff -w -B -b -y --suppress-common-lines --width=170 $ficheroReferencia $ficheroSalida > $ficheroDiff
	    	iguales=$(stat -c%s $ficheroDiff)
        if [ "$ficheroEntrada" = "" ]; then
          ficheroEntrada="(no input file)"
        fi
	    	if [ ${iguales} -eq "0" ]
	    	then
	    		printf "%-35s %-12s %s\n" "$ficheroEntrada" "${VERDE}OK"  "${RESET}"
	    	else
	    		printf "%-35s %-12s %s\n" "$ficheroEntrada" "${ROJO}FAIL" "${RESET}Check ${ficheroReferencia} and ${ficheroSalida}"
	    		allOK=0
        	    if  ${VERBOSE}
    		    then
    		        printf '\nFile: %-77s |   File: %s\n' $ficheroReferencia $ficheroSalida
    		        printf '=%.0s' {1..170}
    		        printf '\n'
        			cat ${ficheroDiff}
        			printf '\n'
          	    fi
	    	fi
	    	rm $ficheroDiff
	    done
    else
   		allOK=0
	    printf "\n${ROJO}Compilation failed${RESET}"
    fi
	printf "\n"
    return ${allOK}
}


function show_result {
    MESSAGE=$1
    RESULT=$2
    printf "${AMARILLO}${MESSAGE} "
    if  [ ${RESULT} -eq 1 ]
    then
        printf "${VERDE}OK${RESET}"
    else
        printf "${ROJO}FAIL${RESET}"
    fi
    printf "\n"
}

#Comprobar que existen en path actual los ficheros output de referencia
#(sino, tal y como está el script da un OK a pesar de mostrar un error en el diff)
for file in ${ficherosRef[@]}
do
	if [ ! -f "${file}.txt" ]
	then
		printf "${ROJO}Please add the reference file ${file}.txt to the current path${RESET}\n"
		exit 1
	fi
done

check_output "Static" "-DSTATIC_LIST static_list.h static_list.c" ${VERBOSE}
STATIC_OK=$?
if [ "$MAIN_NAME" = "test" ]; then
    smesss=" (checkpoint #1 - March 3rd)"
fi
show_result "Static List result${smesss}:" $STATIC_OK

printf "\n\n"

check_output "Dynamic" "-DDYNAMIC_LIST dynamic_list.h dynamic_list.c" ${VERBOSE}
DYNAMIC_OK=$?
if [ "$MAIN_NAME" = "test" ]; then
    smessd=" (checkpoint #2 - March 10th)"
fi
show_result "Dynamic List result${smessd}:" $DYNAMIC_OK

printf "\n\n"


ALL_OK=$((${STATIC_OK} * ${DYNAMIC_OK}))
printf "${AMARILLO}Summary:\n"
printf "========\n"
show_result "Static List result${smesss}:" $STATIC_OK
show_result "Dynamic List result${smessd}:" $DYNAMIC_OK
show_result "Global result:" $ALL_OK
printf "\n"
