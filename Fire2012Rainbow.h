// Fire2012 with programmable Color Palette
// by Mark Kriegsman: https://github.com/FastLED/FastLED/blob/master/examples/Fire2012WithPalette/Fire2012WithPalette.ino
//
// This code is the same fire simulation as the original "Fire2012",
// but each heat cell's temperature is translated to color through a FastLED
// programmable color palette, instead of through the "HeatColor(...)" function.
//
// Four different static color palettes are provided here, plus one dynamic one.
// 
// The three static ones are: 
//   1. the FastLED built-in HeatColors_p -- this is the default, and it looks
//      pretty much exactly like the original Fire2012.
//
//  To use any of the other palettes below, just "uncomment" the corresponding code.
//
//   2. a gradient from black to red to yellow to white, which is
//      visually similar to the HeatColors_p, and helps to illustrate
//      what the 'heat colors' palette is actually doing,
//   3. a similar gradient, but in blue colors rather than red ones,
//      i.e. from black to blue to aqua to white, which results in
//      an "icy blue" fire effect,
//   4. a simplified three-step gradient, from black to red to white, just to show
//      that these gradients need not have four components; two or
//      three are possible, too, even if they don't look quite as nice for fire.
//
// The dynamic palette shows how you can change the basic 'hue' of the
// color palette every time through the loop, producing "rainbow fire".

// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
//// 
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation, 
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on MATRIX_HEIGHT; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking. 
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 55, suggested range 20-100 
#define COOLINGRainbow  150 // 100 // 55 // 86

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKINGRainbow 60 // 30 // 120 // 90 // 60
uint16_t Fire2012Rainbow()
{
    // Array of temperature readings at each simulation cell
    static byte heat[MATRIX_WIDTH][MATRIX_HEIGHT];

    for(uint8_t x = 0; x < MATRIX_WIDTH; x++) {
      // Step 1.  Cool down every cell a little
      for (int i = 0; i < MATRIX_HEIGHT; i++) {
          heat[x][i] = qsub8(heat[x][i], random8(0, ((COOLINGRainbow * 10) / MATRIX_HEIGHT) + 2));
      }
  
      // Step 2.  Heat from each cell drifts 'up' and diffuses a little
      for (int k = MATRIX_HEIGHT - 1; k >= 2; k--) {
          heat[x][k] = (heat[x][k - 1] + heat[x][k - 2] + heat[x][k - 2]) / 3;
      }
  
      // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
      if (random8() < SPARKINGRainbow) {
          int y = random8(2);
          heat[x][y] = qadd8(heat[x][y], random8(160, 255));
      }
  
      // Step 4.  Map from heat cells to LED colors
      for (int j = 0; j < MATRIX_HEIGHT; j++) {
          // Scale the heat value from 0-255 down to 0-240
          // for best results with color palettes.
          byte colorindex = scale8(heat[x][j], 160);
          leds[XY(x, (MATRIX_HEIGHT - 1) - j)] = ColorFromPalette(LavaColors_p, colorindex);
      }
    }

    return 15;
}
