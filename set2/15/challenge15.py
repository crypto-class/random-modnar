from Crypto.Cipher import AES

BS = AES.block_size
pad = lambda s: s + (BS - len(s) % BS) * chr(BS - len(s) % BS)

def pkcs7unpad(raw):
  pad = ord(raw[-1])
  if raw[-pad:] != chr(pad) * pad:
    raise Exception('Bad padding')
    # return False
  return raw[:-pad]

msg = "ICE ICE BABY"

String = "ICE ICE BABY\x04\x04\x04\x04"
String1 = "ICE ICE BABY\x05\x05\x05\x05"
String2 = "ICE ICE BABY\x01\x02\x03\x04"

print pkcs7unpad(String)
print pkcs7unpad(String1)
print pkcs7unpad(String2)