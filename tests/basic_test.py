import ctypes
from ctypes import c_int, c_char_p, POINTER

# Load the DLL
textspotter = ctypes.CDLL('python/lib/textspotter.dll')

# Define the return type and argument types of the function
# The function returns a bool and takes three arguments: two strings and two pointers to int
textspotter.match_text_from_file.restype = ctypes.c_bool
textspotter.match_text_from_file.argtypes = [c_char_p, c_char_p, POINTER(c_int), POINTER(c_int)]

# Prepare the arguments
image_path = "C:\\Users\\P015-TAS-PC\\Pictures\\home.png".encode('utf-8')  # Encode to bytes for C
target_text = "text_to_match".encode('utf-8')  # Encode to bytes for C
x = c_int()
y = c_int()

# Call the function
result = textspotter.match_text_from_file(image_path, target_text, ctypes.byref(x), ctypes.byref(y))

# Check the result
if result:
    print(f"Match found at position: ({x.value}, {y.value})")
else:
    print("No match found.")