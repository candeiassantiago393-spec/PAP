import qrcode

url = "https://www.instagram.com/candeias.dev"
qr = qrcode.QRCode(
    version=None,
    error_correction=qrcode.constants.ERROR_CORRECT_M,
    box_size=1,
    border=1,
)
qr.add_data(url)
qr.make(fit=True)
matrix = qr.get_matrix()
size = len(matrix)
target = 48
scale = max(1, target // size)
out_h = size * scale
out_w = size * scale
pixels = [[0] * out_w for _ in range(out_h)]
for r, row in enumerate(matrix):
    for c, val in enumerate(row):
        for dr in range(scale):
            for dc in range(scale):
                pixels[r * scale + dr][c * scale + dc] = 255 if val else 0

w, h = out_w, out_h
byte_width = (w + 7) // 8
data = []
for y in range(h):
    for bx in range(byte_width):
        b = 0
        for bit in range(8):
            x = bx * 8 + bit
            if x < w and pixels[y][x]:
                b |= 0x80 >> bit
        data.append(b)

print(f"// QR {w}x{h} for {url}")
print(f"#define QR_W {w}")
print(f"#define QR_H {h}")
print("const uint8_t PROGMEM instagramQr[] = {")
for i in range(0, len(data), 12):
    chunk = data[i : i + 12]
    print("  " + ", ".join(f"0x{b:02X}" for b in chunk) + ",")
print("};")
print(f"// {len(data)} bytes")
