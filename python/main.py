import ctypes
import cv2
from ctypes import c_int, c_char_p, POINTER

# Load the DLL
textspotter = ctypes.CDLL('python/textspotter/lib/textspotter.dll')

# Define the return type and argument types of the function
# The function returns a bool and takes three arguments: two strings and two pointers to int
textspotter.match_text_from_file.restype = ctypes.c_bool
textspotter.match_text_from_file.argtypes = [c_char_p, c_char_p, POINTER(c_int), POINTER(c_int)]

# Prepare the arguments
image_path = "C:\\Users\\P015-TAS-PC\\Pictures\\home.png".encode('utf-8')  # Encode to bytes for C
target = input("Enter target:")
target_text = target.encode('utf-8')  # Encode to bytes for C
x = c_int()
y = c_int()

# Call the function
result = textspotter.match_text_from_file(image_path, target_text, ctypes.byref(x), ctypes.byref(y))

# Check the result
if result:
    print(f"Match found at position: ({x.value}, {y.value})")
else:
    print("No match found.")

img = cv2.imread(image_path.decode('utf-8'))
cv2.line(img, (x.value, 0), (x.value, img.shape[0]), (0, 0, 255), 2)
cv2.line(img, (0, y.value), (img.shape[1], y.value), (0, 0, 255), 2)

cv2.imshow("Image", img)
cv2.waitKey(0)
cv2.destroyAllWindows()