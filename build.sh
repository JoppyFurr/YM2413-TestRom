#!/bin/sh
echo ""
echo "YM2413 Test ROM Build Script"
echo "-------------------------------"

sdcc="${HOME}/Code/sdcc-4.3.0/bin/sdcc"
devkitSMS="${HOME}/Code/devkitSMS"
SMSlib="${devkitSMS}/SMSlib"
ihx2sms="${devkitSMS}/ihx2sms/Linux/ihx2sms"
sneptile="./tools/Sneptile-0.1.0/Sneptile"

build_sneptile ()
{
    # Early return if we've already got an up-to-date build
    if [ -e $sneptile -a "./tools/Sneptile-0.1.0/source/main.c" -ot $sneptile ]
    then
        return
    fi

    echo "Building Sneptile..."
    (
        cd "tools/Sneptile-0.1.0"
        ./build.sh
    )
}

build_ym2413_test_rom ()
{
    echo "Building YM2413 Test ROM..."

    echo "  Generating tile data..."
    mkdir -p tile_data
    (
        # Index 0 is used for transparency, use dark grey, our background colour.
        # Index 1, 2, and 3, are used for the cursor colour-cycle.
        # Index 4 is used for the selected key colour.
        $sneptile --output tile_data --palette 0x15 0x01 0x02 0x03 0x33 \
            tiles/empty.png \
            tiles/button.png \
            tiles/cursor.png \
            tiles/digits.png \
            tiles/footer.png \
            tiles/keys.png \
            tiles/led.png \
            tiles/melody_labels.png \
            tiles/names.png \
            tiles/rhythm_labels.png \
            tiles/tabs.png \
            tiles/title.png
    )

    mkdir -p build
    echo "  Compiling..."
    for file in cursor draw register main
    do
        echo " -> ${file}.c"
        ${sdcc} -c -mz80 --peep-file ${devkitSMS}/SMSlib/src/peep-rules.txt -I ${SMSlib}/src \
            -o "build/${file}.rel" "source/${file}.c" || exit 1
    done

    echo ""
    echo "Linking..."
    ${sdcc} -o build/YM2413_TestRom.ihx -mz80 --no-std-crt0 --data-loc 0xC000 ${devkitSMS}/crt0/crt0_sms.rel build/*.rel ${SMSlib}/SMSlib.lib || exit 1

    echo ""
    echo "Generating ROM..."
    ${ihx2sms} build/YM2413_TestRom.ihx YM2413_TestRom.sms || exit 1

    echo ""
    echo "Done"
}

# Clean up any old build artefacts
rm -rf build

build_sneptile
build_ym2413_test_rom
