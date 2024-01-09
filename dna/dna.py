import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage python dna.py data.csv sequence.txt")

    # TODO: Read database file into a variable
    database = []

    with open(sys.argv[1]) as file:
        file_reader = csv.DictReader(file)
        for name in file_reader:
            database.append(name)

    # TODO: Read DNA sequence file into a variable
    sequence = ""

    with open(sys.argv[2]) as file:
        sequence += file.read()

    # TODO: Find longest match of each STR in DNA sequence
    STR = {
        "AGATC": 0,
        "TTTTTTCT": 0,
        "AATG": 0,
        "TCTAG": 0,
        "GATA": 0,
        "TATC": 0,
        "GAAA": 0,
        "TCTG": 0,
    }

    for str in STR:
        STR[str] = longest_match(sequence,str)

    # TODO: Check database for matching profiles

    person = "No match"

    for name in database:
        if match(name, STR):
            person = name["name"]

    print(person)
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


def match(name, STR):
    matching = True

    for str in STR:
        try:
            if STR[str] != int(name[str]):
                matching = False
                return matching

        except KeyError:
            continue

    return matching


main()
