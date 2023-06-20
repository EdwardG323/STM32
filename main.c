* (volatile uint32_t *) (0x42020000 + 0) = 0;	// Sets pins A0 - A15 to inputs

* (volatile uint32_t *) (0x42020000 + 0) &= 3 << 6;	// Clear bit ranges 6-7
* (volatile uint32_t *) (0x42020000 + 0) |= 1 << 6;	// set bit range 6-7 to 1