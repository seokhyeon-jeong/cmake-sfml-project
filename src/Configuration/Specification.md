## Window W H FL FS
----
    - W	width
    - H	height
	- FL	frame limit
	- FS	full screen mode( 1 or 0 )

## Font F S R G B
---
    - F     font file   std::string
    - S     font size   int
    - RGB   color       int

## Player SR CR S FR FG FB OR OG OB OT V
---
    - SR        shape radius        int
    - CR        collision radius    int
    - S         speed               float
    - FRGB      fill color          int
    - ORGB      outlint color       int
    - OT        outline thickness   int
    - V         shape vertices      int

## Enemy SR CR SMIN SMAX OR OG OB OT VMIN VMAX L SI
---
    - SR        shape radius        int
    - CR        collison radius     int
    - SMIN/MAX  min,max speed       float
    - ORGB      outline color       int
    - OT        outline thickness   int
    - VMIN/MAX  min,max vertices    int
    - L         lifespan            int
    - SP        spawan interval     int

## Bullet SR CR S FR FG FB OR OG OB OT V L
---
    - SR        shape radius        int
    - CR        collison radius     int
    - S         speed               float
    - ORGB      outline color       int
    - OT        outline thickness   int
    - V         vertices            int
    - L         lifespan            int
