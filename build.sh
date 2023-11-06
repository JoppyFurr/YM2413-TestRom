#!/bin/sh
echo ""
echo "YM2413 Test ROM Build Script"
echo "-------------------------------"

sdcc="${HOME}/Code/sdcc-4.2.0/bin/sdcc"
devkitSMS="${HOME}/Code/devkitSMS"
SMSlib="${devkitSMS}/SMSlib"
ihx2sms="${devkitSMS}/ihx2sms/Linux/ihx2sms"

build_ym2413_test_rom ()
{
    echo "Building YM2413 Test ROM..."

    mkdir -p build
    echo "  Compiling..."
    for file in main
    do
        echo " -> ${file}.c"
        ${sdcc} -c -mz80 --peep-file ${devkitSMS}/SMSlib/src/peep-rules.txt -I ${SMSlib}/src \
            -o "build/${file}.rel" "source/${file}.c" || exit 1
    done

    echo ""
    echo "Linking..."
    ${sdcc} -o build/ym2413_test_rom.ihx -mz80 --no-std-crt0 --data-loc 0xC000 ${devkitSMS}/crt0/crt0_sms.rel build/*.rel ${SMSlib}/SMSlib.lib || exit 1

    echo ""
    echo "Generating ROM..."
    ${ihx2sms} build/ym2413_test_rom.ihx ym2413_test_rom.sms || exit 1

    echo ""
    echo "Done"
}

# Clean up any old build artefacts
rm -rf build

build_ym2413_test_rom
