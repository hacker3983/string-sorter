# String Sorter
String sorter is a command line tool that allows you to sort strings from a file in either alphabetical (ascending) or reverse alphabetical (descending) order.

# Build instructions
```
gcc main.c -o string_sorter
```

# Usage
```bash
Usage: ./string_sorter [INPUT FILE] [ORDER]...
Sorts strings in a given input file based on the specified order.
If no order is provided, it sorts in alphabetical (ascending) order.

Order:
a - sorts in alphabetical (ascending) order
d - sorts in reverse alphabetical (descending) order

Note: The input file should contain one string per line.
```

# Examples
Sort strings in ascending order (default)
```bash
./string_sorter strings.txt
```

```bash
./string_sorter strings.txt d
```

# How String Sorter works
String sorter uses the Radix Sort algorithm - A comparison-based sorting algorithm. Instead of comparing elements directly like quicksort or mergesort, radix sort uses buckets to group elements based on individual characters (or digits in the case of numbers).

# Radix Sort Explained
Let's say we want to sort numbers that are all the same length. Radix sort would:
1. Create 10 buckets for digits 0-9

2. Iterate through the digits of each number, one digit at a time (either from right to left or left to right).

3. Place each number into the appropriate bucket based on the current digit.

4. Gather the numbers back in order after each pass though the digits.

This process repeats for each digit position until all numbers are sorted.
