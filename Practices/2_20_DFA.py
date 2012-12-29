#!/usr/bin/env python

import FiniteAuto as FA

Trans = FA.Trans

fa = FA.FiniteAuto()
ts = [
		Trans( 1, 2, "/"),
		Trans( 2, 3, "*"),
		Trans( 3, 3, "not *"),
		Trans( 3, 4, "*"),
		Trans( 4, 4, "*"),
		Trans( 4, 3, "not */"),
		Trans( 4, 5, "/", True) ]

fa.addTrans( ts);
fa.draw()
