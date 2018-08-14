require 'physics.magnitude-order'

Class "Phasor"

function Phasor:constructor(nAmplitude, nPhase, bDegrees)
	self._amplitude = nAmplitude
	self._phase = (nPhase or 0) / (bDegrees and (180 * math.pi) or 1)
end

function Phasor:amplitude()
	return self._amplitude
end

function Phasor:phase(bDegrees)
	return self._phase * (bDegrees and (180 / math.pi) or 1)
end

function Phasor:real()
	return math.cos(self._phase) * self._amplitude
end

function Phasor:imaginary()
	return math.sin(self._phase) * self._amplitude
end

function Phasor:__add(cPhasor_or_nReal)

	if type(self) == 'number' then
		self, cPhasor_or_nReal = cPhasor_or_nReal, self
	end

	local real, imaginary

	if type(cPhasor_or_nReal) == 'number' then
		real, imaginary = cPhasor_or_nReal, 0
	else
		real, imaginary = cPhasor_or_nReal:real(), cPhasor_or_nReal:imaginary()
	end

	real = self:real() + real
	imaginary = self:imaginary() + imaginary

	local amplitude = (real^2 + imaginary^2)^.5

	if real == 0 then
		return Phasor(amplitude, imaginary >= 0 and (math.pi / 2) or -(math.pi / 2))
	end

	return Phasor(amplitude, math.atan(imaginary / real))
end

function Phasor:__sub(cPhasor_or_nReal)

	if type(self) == 'number' then
		self, cPhasor_or_nReal = -cPhasor_or_nReal, -self
	end

	local real, imaginary

	if type(cPhasor_or_nReal) == 'number' then
		real, imaginary = cPhasor_or_nReal, 0
	else
		real, imaginary = cPhasor_or_nReal:real(), cPhasor_or_nReal:imaginary()
	end

	real = self:real() - real
	imaginary = self:imaginary() - imaginary

	local amplitude = (real^2 + imaginary^2)^.5

	if real == 0 then
		return Phasor(amplitude, imaginary >= 0 and (math.pi / 2) or -(math.pi / 2))
	end

	return Phasor(amplitude, math.atan(imaginary / real))
end

function Phasor:__mul(cPhasor_or_nReal)

	if type(self) == 'number' then
		self, cPhasor_or_nReal = cPhasor_or_nReal, self
	end

	local amplitude, phase

	if type(cPhasor_or_nReal) == 'number' then
		amplitude, phase = cPhasor_or_nReal, 0
	else
		amplitude, phase = cPhasor_or_nReal._amplitude, cPhasor_or_nReal._phase
	end

	return Phasor(self._amplitude * amplitude, self._phase + phase)
end

function Phasor:__div(cPhasor_or_nReal)

	if type(self) == 'number' then
		self, cPhasor_or_nReal = cPhasor_or_nReal^(-1), self^(-1)
	end

	local amplitude, phase

	if type(cPhasor_or_nReal) == 'number' then
		amplitude, phase = cPhasor_or_nReal, 0
	else
		amplitude, phase = cPhasor_or_nReal._amplitude, cPhasor_or_nReal._phase
	end

	return Phasor(self._amplitude / amplitude, self._phase - phase)
end

function Phasor:__pow(nPower)
	return Phasor(self._amplitude^nPower, self._phase * nPower)
end

function Phasor:__tostring()
	local amplitude, order = MagnitudeOrder.normalize(self._amplitude)
	return string.format("%.3f%s/%.2f", amplitude, order, (self._phase / math.pi * 180))
end

function Phasor:__concat(other)
	return tostring(self) .. tostring(other)
end

function Phasor:__unm(cPhasor)
	return Phasor(-cPhasor._amplitude, cPhasor._phase)
end

Phasor.fromComplex = function(nReal, nImaginary)
	return Phasor((nReal^2 + nImaginary^2)^0.5,
		nReal == 0 and (nImaginary < 0 and -math.pi/2 or math.pi/2) or
		math.atan(nImaginary/nReal))
end

Phasor.Capacitor = function(nCapacitance, nAngularFrequency)
	return Phasor.fromComplex(0, -1/nCapacitance/nAngularFrequency)
end

Phasor.Inductor = function(nInductance, nAngularFrequency)
	return Phasor.fromComplex(0, nInductance * nAngularFrequency)
end

Phasor.Resistor = function(nResistance)
	return Phasor(nResistance, 0)
end

return Pashor
