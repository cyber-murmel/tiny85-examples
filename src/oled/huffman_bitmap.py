#!/ust/bin/env

import sys

size = [128, 48]

if len(sys.argv) != 3:
    exit()

bin_file = sys.argv[1]
h_file = sys.argv[2]

# generate huffman tree for given data
def huffman_tree(data):
    huffman_set = {}

    for char in data:
        if not char in huffman_set:
            huffman_set[char] = 1
        else:
            huffman_set[char] += 1

    huffman_trees = list(huffman_set.items())

    while len(huffman_trees) > 1:
        huffman_trees = sorted(huffman_trees, key=lambda item: item[1])
        new_tree = [
            [
                huffman_trees[0][0],
                huffman_trees[1][0],
            ],
            huffman_trees[0][1] + huffman_trees[1][1]
        ]
        huffman_trees = huffman_trees[2:]
        huffman_trees.append(new_tree)
    
    # get remaining tree
    return huffman_trees[0][0]

def huffman_tree2table(tree, bits=''):
    if isinstance(tree, list):
        table = {}
        for bit in [0, 1]:
            sub_table = huffman_tree2table(tree[bit], str(bit)+bits) # append as most significant bit (LSB is highes int tree)
            table.update(sub_table)
        return table
    else:
        return {tree: bits}
        print(f"{bits}: {tree:02x}")

def huffman_tree2list(tree):
    if isinstance(tree, list):
        sub_list_0 = huffman_tree2list(tree[0])
        sub_list_1 = huffman_tree2list(tree[1])
        index_0 = 1
        index_1 = index_0 + len(sub_list_0)
        sub_list_0 = [[entry[0]+index_0, entry[1]+index_0] if entry[0] != entry[1] else entry for entry in sub_list_0]
        sub_list_1 = [[entry[0]+index_1, entry[1]+index_1] if entry[0] != entry[1] else entry for entry in sub_list_1]
        tree = [[index_0, index_1]] + sub_list_0 + sub_list_1
        return tree
    else:
        return [[tree, tree]] # two same values = actual value

def huffman_list2tree(h_list):
    val0 = h_list[0][0]
    val1 = h_list[0][1]
    if val0 != val1:
        sub_list_0 = [[entry[0]-val0, entry[1]-val0] if entry[0] != entry[1] else entry for entry in h_list[val0:val1]]
        sub_list_1 = [[entry[0]-val1, entry[1]-val1] if entry[0] != entry[1] else entry for entry in h_list[val1:]]
        return [
            huffman_list2tree(sub_list_0),
            huffman_list2tree(sub_list_1),
        ]
    else:
        return val0

def huffman_code(data, table):
    code = b''
    bits = ''
    for byte in data:
        bits = table[byte] + bits

    while bits:
        chunk_len = min(8, len(bits))
        chunk = bits[-chunk_len:]
        code += int(chunk, 2).to_bytes()
        bits = bits[:-chunk_len]

    return code

def huffman_decode(code, tree, length):
    data = b''
    bit_index = 0
    code_byte = code[0]
    tree_pointer = tree
    while len(data) < length:
        tree_pointer = tree_pointer[code_byte & 1]

        if isinstance(tree_pointer, int):
            data += tree_pointer.to_bytes()
            tree_pointer = tree
            
        if bit_index >=7 and (len(data) < length):
            code = code[1:]
            code_byte = code[0]
            bit_index = 0
        else:
            bit_index += 1
            code_byte >>= 1
    return data

def test():
    test_data = b'aaaaaaaaaaabbbbcccdd'

    test_tree = huffman_tree(test_data)
    print(test_tree)

    test_table= huffman_tree2table(test_tree)
    print(test_table)

    test_code = huffman_code(test_data, test_table)
    print("\n".join([f"{byte:08b}" for byte in test_code]))

    test_decode = huffman_decode(test_code, test_tree, len(test_data))
    print(test_decode)

    test_list = huffman_tree2list(test_tree)
    print(test_list)

    print(huffman_list2tree(test_list))

# test()
# exit()

with open(bin_file, 'rb') as input_file, open(h_file, 'w') as output_file:
    bytes_per_frame = size[0]*size[1]//8
    data = input_file.read()

    data_tree = huffman_tree (data)
    data_table = huffman_tree2table(data_tree)
    data_tree_list = huffman_tree2list(data_tree)
    if data_tree != huffman_list2tree(data_tree_list):
        print("tree mismatch")
        exit(-1)

    print(len(data_table))
    # print(data_table)
    # print(data_tree_list)
    print(len(data_tree_list))

    frames = [data[i:i+bytes_per_frame] for i in range(0, len(data), bytes_per_frame)]
    print(len(frames))
    index = 0
    for frame in frames:
        huffman_frame = huffman_code(frame, data_table)

        if frame != huffman_decode(huffman_frame, data_tree, len(frame)):
            print("decode mismatch")
            exit(-1)        

        print(f"{len(huffman_frame)}/{len(frame)} : {len(huffman_frame)/len(frame):.02}")


        output_file.write(f"const unsigned char huffman_bitmap_128x64_{index:03} [] PROGMEM = {{\n")
        output_file.write(", ".join([f"0x{char:02x}" for char in huffman_frame]))
        output_file.write(f"\n}};\n")

        index += 1

    output_file.write(f"const int num_bitmaps = {index};\n")
    output_file.write(f"const unsigned char* huffman_bitmaps[{index}] = {{\n")
    for i in range(index):
        output_file.write(f"    huffman_bitmap_128x64_{i:03},\n")
    output_file.write(f"}};\n")

    output_file.write(f"const unsigned char huffman_table_list[][2] PROGMEM = {{\n")
    for pair in data_tree_list:
        output_file.write(f"    {{ {pair[0]}, {pair[1]} }},\n")
    output_file.write(f"}};\n")
