import requests
import idna
import os

# Custom TLDs to add at the beginning of the list
CUSTOM_TLDS = ["local"]

# URL of the TLD list from IANA
URL = "https://data.iana.org/TLD/tlds-alpha-by-domain.txt"
# Output files
RAW_FILE = "tlds-alpha-by-domain.txt"
UTF8_FILE = "tlds-utf8-lowercase.txt"
HEADER_FILE = "src/iana-tlds.h"

# Download the file if it doesn't exist or is empty
def download_tld_list():
    if not os.path.exists(RAW_FILE) or os.path.getsize(RAW_FILE) == 0:
        print(f"Downloading file from {URL}...")
        response = requests.get(URL)
        if response.status_code == 200:
            with open(RAW_FILE, "w", encoding="utf-8") as file:
                file.write(response.text)
            print(f"File saved as {RAW_FILE}.")
        else:
            print(f"Error downloading the list: {response.status_code}")
            exit(1)
    else:
        print(f"File {RAW_FILE} already exists. Will not download again.")

# Convert content to UTF-8 and save to a separate file
def convert_to_utf8():
    if not os.path.exists(RAW_FILE):
        print(f"File {RAW_FILE} does not exist. Please download it first.")
        exit(1)

    with open(RAW_FILE, "r", encoding="utf-8") as infile, open(UTF8_FILE, "w", encoding="utf-8") as outfile:
        # First, write custom TLDs at the beginning
        print(f"Adding {len(CUSTOM_TLDS)} custom TLDs to the list...")
        for custom_tld in CUSTOM_TLDS:
            outfile.write(custom_tld.lower() + "\n")

        # Then process the IANA TLD list
        for line in infile:
            line = line.strip()
            # Ignore comments and empty lines
            if not line or line.startswith("#"):
                continue

            lower_case = line.lower()
            if lower_case.startswith("xn--"):
                try:
                    decoded = idna.decode(lower_case)
                    outfile.write(decoded + "\n")
                except idna.IDNAError:
                    outfile.write(lower_case + "\n")
            else:
                outfile.write(lower_case + "\n")

    print(f"File converted and saved as {UTF8_FILE} (custom TLDs + IANA TLDs).")

# Create C++ header file
def create_header_file():
    if not os.path.exists(UTF8_FILE):
        print(f"File {UTF8_FILE} does not exist. Please convert it first.")
        exit(1)

    with open(UTF8_FILE, "r", encoding="utf-8") as infile, open(HEADER_FILE, "w", encoding="utf-8") as outfile:

        outfile.write("// -----------------------------------------------------------\n");
        outfile.write("//\n");
        outfile.write("// Full list of TLDs: https://www.iana.org/domains/root/db\n");
        outfile.write("// Source used for the include: https://data.iana.org/TLD/\n");
        outfile.write("// The TLD_LIST excludes \"test\" TLDs found in the full list.\n");
        outfile.write("// Entries converted already to UTF-8 from Punycode.\n");
        outfile.write(f"// Custom TLDs added: {', '.join(CUSTOM_TLDS)}\n");
        outfile.write("// Updated 2025-01-22 08:07\n");
        outfile.write("\n");
        outfile.write("// If you need to update the TLD list, use this repository:\n");
        outfile.write("// https://github.com/LuisPalacios/tldcheck\n");
        outfile.write("\n");
        outfile.write("// -----------------------------------------------------------\n");
        outfile.write("\n");
        outfile.write("#ifndef IANA_TLDS_H\n")
        outfile.write("#define IANA_TLDS_H\n\n")
        outfile.write("#include <set>\n")
        outfile.write("#include <string>\n\n")
        outfile.write("static const std::set<std::string> TLD_LIST = {\n")

        first = True
        for line in infile:
            line = line.strip()
            if not first:
                outfile.write(",\n")
            outfile.write(f"    \"{line}\"")
            first = False

        outfile.write("\n};\n\n")
        outfile.write("#endif // IANA_TLDS_H\n")

    # Count total TLDs in the file
    with open(UTF8_FILE, "r", encoding="utf-8") as count_file:
        total_tlds = sum(1 for line in count_file if line.strip())

    print(f"Header file created as {HEADER_FILE}.")
    print(f"Total TLDs in list: {total_tlds} (including {len(CUSTOM_TLDS)} custom TLDs)")

if __name__ == "__main__":
    download_tld_list()
    convert_to_utf8()
    create_header_file()