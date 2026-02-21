#include <sat.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdio.h>

// Helper function to check if a character is a valid hex digit
static bool is_hex_digit (char c)
{
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

// Helper function to validate UUID string format
static bool is_valid_uuid_format (const sat_uuid_string_t uuid, sat_uuid_format_t format)
{
    const char *str = (const char *)uuid;
    
    // Check length
    if (strlen (str) != 36)
    {
        return false;
    }
    
    // Check format: XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX
    for (int i = 0; i < 36; i++)
    {
        if (i == 8 || i == 13 || i == 18 || i == 23)
        {
            if (str[i] != '-')
            {
                return false;
            }
        }
        else
        {
            if (!is_hex_digit (str[i]))
            {
                return false;
            }
            
            // Check case format
            if (format == sat_uuid_format_upper_case && str[i] >= 'a' && str[i] <= 'f')
            {
                return false;
            }
            if (format == sat_uuid_format_lower_case && str[i] >= 'A' && str[i] <= 'F')
            {
                return false;
            }
        }
    }
    
    return true;
}

// Test sat_uuid_generate_string with upper case format
static void test_uuid_generate_string_upper_case (void)
{
    sat_uuid_string_t uuid = {0};
    bool result = sat_uuid_generate_string (uuid, sat_uuid_format_upper_case);
    
    assert (result == true);
    assert (strlen ((char *)uuid) == 36);
    assert (is_valid_uuid_format (uuid, sat_uuid_format_upper_case));
    
    printf ("✓ UUID generate string (upper case): %s\n", uuid);
}

// Test sat_uuid_generate_string with lower case format
static void test_uuid_generate_string_lower_case (void)
{
    sat_uuid_string_t uuid = {0};
    bool result = sat_uuid_generate_string (uuid, sat_uuid_format_lower_case);
    
    assert (result == true);
    assert (strlen ((char *)uuid) == 36);
    assert (is_valid_uuid_format (uuid, sat_uuid_format_lower_case));
    
    printf ("✓ UUID generate string (lower case): %s\n", uuid);
}

// Test sat_uuid_generate_bin
static void test_uuid_generate_bin (void)
{
    sat_uuid_binary_t uuid_bin = {0};
    bool result = sat_uuid_generate_bin (uuid_bin);
    
    assert (result == true);
    
    // Check that binary data is not all zeros (very unlikely for a real UUID)
    bool all_zeros = true;
    for (int i = 0; i < SAT_UUID_BINARY_SIZE; i++)
    {
        if (uuid_bin[i] != 0)
        {
            all_zeros = false;
            break;
        }
    }
    assert (!all_zeros);
    
    printf ("✓ UUID generate binary: ");
    for (int i = 0; i < SAT_UUID_BINARY_SIZE; i++)
    {
        printf ("%02x", uuid_bin[i]);
    }
    printf ("\n");
}

// Test sat_uuid_string_to_bin
static void test_uuid_string_to_bin (void)
{
    sat_uuid_string_t uuid_string = {0};
    sat_uuid_binary_t uuid_bin = {0};
    
    // First generate a string UUID
    bool result = sat_uuid_generate_string (uuid_string, sat_uuid_format_upper_case);
    assert (result == true);
    
    // Convert string to binary
    result = sat_uuid_string_to_bin (uuid_string, uuid_bin);
    assert (result == true);
    
    // Check that binary data is not all zeros
    bool all_zeros = true;
    for (int i = 0; i < SAT_UUID_BINARY_SIZE; i++)
    {
        if (uuid_bin[i] != 0)
        {
            all_zeros = false;
            break;
        }
    }
    assert (!all_zeros);
    
    printf ("✓ UUID string to binary conversion\n");
}

// Test sat_uuid_bin_to_string
static void test_uuid_bin_to_string (void)
{
    sat_uuid_binary_t uuid_bin = {0};
    sat_uuid_string_t uuid_string = {0};
    
    // First generate a binary UUID
    bool result = sat_uuid_generate_bin (uuid_bin);
    assert (result == true);
    
    // Convert binary to string
    result = sat_uuid_bin_to_string (uuid_bin, uuid_string, sat_uuid_format_upper_case);
    assert (result == true);
    
    // Validate the string format
    assert (strlen ((char *)uuid_string) == 36);
    assert (is_valid_uuid_format (uuid_string, sat_uuid_format_upper_case));
    
    printf ("✓ UUID binary to string conversion: %s\n", uuid_string);
}

// Test round-trip conversion (string -> binary -> string)
static void test_uuid_round_trip_conversion (void)
{
    sat_uuid_string_t original_string = {0};
    sat_uuid_string_t final_string = {0};
    sat_uuid_binary_t uuid_bin = {0};

    // Generate original string
    bool result = sat_uuid_generate_string (original_string, sat_uuid_format_upper_case);
    assert (result == true);
    
    // Convert to binary
    result = sat_uuid_string_to_bin (original_string, uuid_bin);
    assert (result == true);
    
    // Convert back to string
    result = sat_uuid_bin_to_string (uuid_bin, final_string, sat_uuid_format_upper_case);
    assert (result == true);
    
    // Compare original and final strings
    assert (strcmp ((char *)original_string, (char *)final_string) == 0);
    
    printf ("✓ UUID round-trip conversion: %s\n", original_string);
}

// Test sat_uuid_compare_bin with equal UUIDs
static void test_uuid_compare_bin_equal (void)
{
    sat_uuid_binary_t uuid1 = {0};
    sat_uuid_binary_t uuid2 = {0};

    // Generate one binary UUID and copy it to both
    bool result = sat_uuid_generate_bin (uuid1);
    assert (result == true);

    memcpy (uuid2, uuid1, SAT_UUID_BINARY_SIZE);

    assert (sat_uuid_compare_bin (uuid1, uuid2) == true);

    printf ("✓ UUID compare binary (equal)\n");
}

// Test sat_uuid_compare_bin with different UUIDs
static void test_uuid_compare_bin_different (void)
{
    sat_uuid_binary_t uuid1 = {0};
    sat_uuid_binary_t uuid2 = {0};

    bool result1 = sat_uuid_generate_bin (uuid1);
    bool result2 = sat_uuid_generate_bin (uuid2);
    assert (result1 == true);
    assert (result2 == true);

    // Two independently generated UUIDs are practically never equal
    assert (sat_uuid_compare_bin (uuid1, uuid2) == false);

    printf ("✓ UUID compare binary (different)\n");
}

// Test sat_uuid_compare_string with equal UUIDs
static void test_uuid_compare_string_equal (void)
{
    sat_uuid_string_t uuid1 = {0};
    sat_uuid_string_t uuid2 = {0};

    bool result = sat_uuid_generate_string (uuid1, sat_uuid_format_upper_case);
    assert (result == true);

    // Copy the same UUID string to both buffers
    memcpy (uuid2, uuid1, SAT_UUID_STRING_SIZE);

    assert (sat_uuid_compare_string (uuid1, uuid2, sat_uuid_format_upper_case) == true);

    printf ("✓ UUID compare string (equal)\n");
}

// Test sat_uuid_compare_string with different UUIDs
static void test_uuid_compare_string_different (void)
{
    sat_uuid_string_t uuid1 = {0};
    sat_uuid_string_t uuid2 = {0};

    bool result1 = sat_uuid_generate_string (uuid1, sat_uuid_format_upper_case);
    bool result2 = sat_uuid_generate_string (uuid2, sat_uuid_format_upper_case);
    assert (result1 == true);
    assert (result2 == true);

    assert (sat_uuid_compare_string (uuid1, uuid2, sat_uuid_format_upper_case) == false);

    printf ("✓ UUID compare string (different)\n");
}

// Test sat_uuid_string_from_string with a valid input
static void test_uuid_string_from_string_valid (void)
{
    const char *input = "550e8400-e29b-41d4-a716-446655440000";
    sat_uuid_string_t uuid_upper = {0};
    sat_uuid_string_t uuid_lower = {0};

    bool result = sat_uuid_string_from_string (input, uuid_upper, sat_uuid_format_upper_case);
    assert (result == true);
    assert (is_valid_uuid_format (uuid_upper, sat_uuid_format_upper_case));

    result = sat_uuid_string_from_string (input, uuid_lower, sat_uuid_format_lower_case);
    assert (result == true);
    assert (is_valid_uuid_format (uuid_lower, sat_uuid_format_lower_case));

    printf ("✓ UUID string from string (valid): upper=%s lower=%s\n", uuid_upper, uuid_lower);
}

// Test sat_uuid_string_from_string with an invalid input
static void test_uuid_string_from_string_invalid (void)
{
    const char *invalid_input = "not-a-valid-uuid";
    sat_uuid_string_t uuid = {0};

    bool result = sat_uuid_string_from_string (invalid_input, uuid, sat_uuid_format_upper_case);
    assert (result == false);

    printf ("✓ UUID string from string (invalid input rejected)\n");
}

// Test sat_uuid_binary_from_string with a valid input
static void test_uuid_binary_from_string_valid (void)
{
    const char *input = "550e8400-e29b-41d4-a716-446655440000";
    sat_uuid_binary_t uuid_bin = {0};

    bool result = sat_uuid_binary_from_string (input, uuid_bin);
    assert (result == true);

    // The binary representation must not be all zeros for this non-zero UUID
    bool all_zeros = true;
    for (int i = 0; i < SAT_UUID_BINARY_SIZE; i++)
    {
        if (uuid_bin [i] != 0)
        {
            all_zeros = false;
            break;
        }
    }
    assert (!all_zeros);

    // Round-trip: convert back to string and verify it matches the original
    sat_uuid_string_t recovered = {0};
    result = sat_uuid_bin_to_string (uuid_bin, recovered, sat_uuid_format_lower_case);
    assert (result == true);
    assert (strcmp (input, (char *)recovered) == 0);

    printf ("✓ UUID binary from string (valid): %s\n", recovered);
}

// Test sat_uuid_binary_from_string with an invalid input
static void test_uuid_binary_from_string_invalid (void)
{
    const char *invalid_input = "not-a-valid-uuid";
    sat_uuid_binary_t uuid_bin = {0};

    bool result = sat_uuid_binary_from_string (invalid_input, uuid_bin);
    assert (result == false);

    printf ("✓ UUID binary from string (invalid input rejected)\n");
}

int main (int argc, char *argv[])
{
    printf ("Running UUID tests...\n\n");

    test_uuid_generate_string_upper_case ();
    test_uuid_generate_string_lower_case ();
    test_uuid_generate_bin ();
    test_uuid_string_to_bin ();
    test_uuid_bin_to_string ();
    test_uuid_round_trip_conversion ();
    test_uuid_compare_bin_equal ();
    test_uuid_compare_bin_different ();
    test_uuid_compare_string_equal ();
    test_uuid_compare_string_different ();
    test_uuid_string_from_string_valid ();
    test_uuid_string_from_string_invalid ();
    test_uuid_binary_from_string_valid ();
    test_uuid_binary_from_string_invalid ();

    printf ("\n✓ All UUID tests passed!\n");

    return 0;
}
