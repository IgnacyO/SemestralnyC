import argparse
import struct
from PIL import Image


parser = argparse.ArgumentParser()
parser.add_argument("--decapsulate", "-d", action="store_true", default=False, required=False)
parser.add_argument("--encapsulate", "-e", action="store_true", default=False, required=False)
parser.add_argument("input_file", help="Path to the input file")
parser.add_argument("output_file", help="Path to the output file")
args = parser.parse_args()

def decapsulate(input_file: str, output_file: str) -> None:
    """This function decapsulates 'input_file' file into Raw Image Data custom format and saves it in
    'output_file'.

    Raw Image Data format file consists of 2 parts:
    1) Length: 4 + 4 = 8 bytes
    - first 4 bytes representing width of the image as unsigned int
    - next 4 bytes  representing height of the image as unsigned int
    
    Struct Format: 'ii'
    
    2) Length: image_size*3 = n*3 bytes where n = image_size = width*height
    - N subsequently represented pixels as 3 subsequent bytes (RGB)
    
    Struct Format: n*'BBB'
    """
    try:
        with Image.open(input_file) as image_file:
            size = image_file.size
            size_b = struct.pack("ii", size[0], size[1])
            data = list(image_file.getdata())
            data_b = bytearray()
            for chunk in data:
                data_b.append(chunk[0])
                data_b.append(chunk[1])
                data_b.append(chunk[2])
            try:
                with open(output_file, "wb") as out_f:
                    out_f.write(size_b)
                    out_f.write(data_b)
            except PermissionError:
                print("[!e]: Not enough permissions to save output into the file.")
                exit(-1)
    except FileNotFoundError:
        print(f"[!e]: No such file or directory: '{input_file}'")
        exit(-1)
    except PermissionError:
        print(f"[!e]: Not enough permissions to open file: '{input_file}'")
        exit(-1)

def encapsulate(input_file: str, output_file: str) -> None:
    """This function encapsulates Raw Image Data and based on loaded rid object creates a new image file based on its data.
    That image file is saved under `output_file` path.

    Args:
        input_file (str): Path to the input file( with extension '.rid')
        output_file (str): Path to the output file(with extension '.png' | '.jpg' | '.jpeg')
    """
    try:
        with open(input_file, "rb") as rid_obj_file:
            
            size_b = rid_obj_file.read(8)
            size = struct.unpack("II", size_b)
            rid_obj_file.seek(8)
            print(size)
            data_b = rid_obj_file.read(size[0]*size[1]*3)
            data = []
            for i in range(0, size[0]*size[1]*3 - 2, 3):
                chunk = (data_b[i], data_b[i+1], data_b[i+2])
                data.append(chunk)
            try:
                new_im = Image.new("RGB", size)
                new_im.putdata(data)
                new_im.save(output_file)
            except PermissionError:
                print("[!e]: Not enough permissions to save output into the file.")
                exit(-1)
    except FileNotFoundError:
        print(f"[!e]: No such file or directory: '{input_file}'")
        exit(-1)
    except PermissionError:
        print(f"[!e]: Not enough permissions to open file: '{input_file}'")
        exit(-1)

def main():
    """Main function."""

    if not args.decapsulate and not args.encapsulate:
        print("[!e] Operation was not specified! Please use '-d' or '-e' flag.")
        exit(-1)

    if args.decapsulate:
        decapsulate(args.input_file, args.output_file)
    else:
        encapsulate(args.input_file, args.output_file)

if __name__ == "__main__":
    main()
