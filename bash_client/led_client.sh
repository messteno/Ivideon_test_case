#! /bin/bash

OPTIND=1

function show_help() {
    echo "Usage: $0 <command> param"
    echo "Commands":
    echo "  -g --get"
    echo "  -s --set"
    echo "Params:"
    echo "  -g state"
    echo "  -s state=on|off"
    echo "  -g color"
    echo "  -s color=red|green|blue"
    echo "  -g rate"
    echo "  -s rate=0..5"
}

while getopts "h?g:s::" opt;
do
    case "$opt" in
        g|--get)
            getcommand=$OPTARG
            ;;
        s|--set)
            setcommand=$OPTARG
            ;;
        h|\?|*)
            show_help
            exit 0
            ;;
    esac
done

infifo=/tmp/l0
outfifo=/tmp/l1

if [ ! -p $outfifo ];
then
    mkfifo $outfifo
fi

if [ ! -z $getcommand ];
then
    command=$getcommand
    case $command in
        state)
            echo "get-led-state" > $infifo
            ;;
        color)
            echo "get-led-color" > $infifo
            ;;
        rate)
            echo "get-led-rate" > $infifo
            ;;
        *)
            echo "Wrong parameter"
            show_help
            exit 0
            ;;
    esac
    cat $outfifo
fi

if [ ! -z $setcommand ];
then
    if [[ ! $setcommand == *"="* ]];
    then
        echo "Wrong parameter"
        show_help
        exit 0
    fi

    command=${setcommand/=*/}
    argument=${setcommand/*=/}

    case $command in
        state)
            echo "set-led-state" $argument > $infifo
            ;;
        color)
            echo "set-led-color" $argument > $infifo
            ;;
        rate)
            echo "set-led-rate" $argument > $infifo
            ;;
        *)
            echo "Wrong command"
            show_help
            exit 0
            ;;
    esac
    cat $outfifo
fi

