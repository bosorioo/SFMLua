local k = {
   0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
   0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
   0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
   0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
   0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
   0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
   0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
   0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
   0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
   0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
   0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
   0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
   0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
   0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
   0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
   0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
}

local HH = {}

local function str2hexa(s)
	local h = string.gsub(s, ".", function(c)
		return string.format("%02x", string.byte(c))
	end)
	return h
end

local function num2s(l, n)
	local s = ""
	for i = 1, n do
		local rem = l % 256
		s = string.char(rem) .. s
		l = (l - rem) / 256
	end
	return s
end

local function s232num (s, i)
	local n = 0
	for i = i, i + 3 do
		n = n * 256 + string.byte(s, i)
	end
	return n
end

local function preproc (msg, len)
	local extra = -(len + 1 + 8) % 64
	len = num2s(8 * len, 8)    -- original len in bits, coded
	msg = msg .. "\128" .. string.rep("\0", extra) .. len
	--assert(#msg % 64 == 0)
	return msg
end

local function initH256(H)
	H[1] = 0x6a09e667
	H[2] = 0xbb67ae85
	H[3] = 0x3c6ef372
	H[4] = 0xa54ff53a
	H[5] = 0x510e527f
	H[6] = 0x9b05688c
	H[7] = 0x1f83d9ab
	H[8] = 0x5be0cd19
	return H
end

local function rrotate(value, bits)
	value = value & 0xFFFFFFFF
	return ((value >> bits) | ((value << (-bits & 31)) & 0xFFFFFFFF))
end

local function bnot(n)
	return (-1 - n) % 2^32
end

local function digestblock (msg, i, H)
	local w = {}
	for j = 1, 16 do
	  w[j] = s232num(msg, i + (j - 1) * 4)
	end
	for j = 17, 64 do
	  local v = w[j - 15] & 0xFFFFFFFF
	  local s0 = rrotate(v, 7) ~ rrotate(v, 18) ~ (v >> 3)
	  v = w[j - 2] & 0xFFFFFFFF
	  local s1 = rrotate(v, 17) ~ rrotate(v, 19) ~ (v >> 10)
	  w[j] = w[j - 16] + s0 + w[j - 7] + s1
	end
	local a, b, c, d, e, f, g, h =
		H[1], H[2], H[3], H[4], H[5], H[6], H[7], H[8]
	for i = 1, 64 do
	  local s0 = rrotate(a, 2) ~ rrotate(a, 13) ~ rrotate(a, 22)
	  local maj = ((a & b) ~ (a & c) ~ (b & c)) & 0xFFFFFFFF
	  local t2 = s0 + maj
	  local s1 = rrotate(e, 6) ~ rrotate(e, 11) ~ rrotate(e, 25)
	  local ch = ((e & f) ~ (bnot(e) & g)) & 0xFFFFFFFF
	  local t1 = h + s1 + ch + k[i] + w[i]
	  h = g
	  g = f
	  f = e
	  e = d + t1
	  d = c
	  c = b
	  b = a
	  a = t1 + t2
	end
	H[1] = (H[1] + a) & 0xFFFFFFFF
	H[2] = (H[2] + b) & 0xFFFFFFFF
	H[3] = (H[3] + c) & 0xFFFFFFFF
	H[4] = (H[4] + d) & 0xFFFFFFFF
	H[5] = (H[5] + e) & 0xFFFFFFFF
	H[6] = (H[6] + f) & 0xFFFFFFFF
	H[7] = (H[7] + g) & 0xFFFFFFFF
	H[8] = (H[8] + h) & 0xFFFFFFFF
end

local function finalresult256 (H)
	local result = {}
	for i = 1, 8 do
		result[#result + 1] = num2s(H[i], 4)
	end
	result = table.concat(result, '')
	return str2hexa(result), result
end

function sha256(str)
	str = preproc(str, #str)
	local H = initH256(HH)
	for i = 1, #str, 64 do
		digestblock(str, i, H)
	end
	return finalresult256(H)
end

bintohex = str2hexa
