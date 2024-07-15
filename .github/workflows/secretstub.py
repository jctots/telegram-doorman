include_file = open('include/secrets.h', 'w')
include_file.write("// ##########################################################################\n")
include_file.write("// ### This file is generated by Build and Continuous Integration scripts ###\n")
include_file.write("// ###   .github/workflows/secretstub.py for CI environment               ###\n")
include_file.write("// ### Changes will be overwritten on the next build                      ###\n")
include_file.write("// ##########################################################################\n")
include_file.write("#pragma once                                    \n")
include_file.write("#define DEFAULT_WIFI_SSID \"xxxyyyzzz\"           \n")
include_file.write("#define DEFAULT_WIFI_PASSWORD \"xxxyyyzzz\"       \n")
include_file.write("#define DEFAULT_OTA_PASSWORD \"xxxyyyzzz\"        \n")
include_file.write("#define BOT_TOKEN \"xxxyyyzzz:xxxyyyzzz\"         \n")
include_file.write("#define DEFAULT_TELEGRAM_CHAT_ID \"xxxyyyzzz\"    \n")
include_file.write("#define DEFAULT_TELEGRAM_PASSWORD \"/xxxyyyzzz\"  \n")
include_file.write("#define DEFAULT_TCS_APT_BELL 0xDEADBEEF         \n")
include_file.write("#define DEFAULT_TCS_STREET_CALL 0xDEADBEEF      \n")
include_file.write("#define DEFAULT_TCS_GARAGE_CALL 0xDEADBEEF      \n")
include_file.write("#define DEFAULT_TCS_STREET_VIEW 0xDEADBEEF      \n")
include_file.write("#define DEFAULT_TCS_GARAGE_VIEW 0xDEADBEEF      \n")
include_file.write("#define DEFAULT_TCS_STREET_OPEN 0xDEADBEEF      \n")
include_file.write("#define DEFAULT_TCS_GARAGE_OPEN 0xDEADBEEF      \n")
include_file.close()