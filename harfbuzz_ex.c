// C Implementation for putpixel()
#include <graphics.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <hb.h>
#include <hb-ft.h>
#include <ft2build.h>
#include FT_FREETYPE_H


#define WIDTH   500
#define HEIGHT  480


/* origin is the upper left corner */
unsigned char image[HEIGHT][WIDTH];
// const uint16_t textMal[] = {0x0d15, 0x0d41, 0x0d1f, 0x0d4d, 0x0d1f, 0x0d3f, 0x0d15, 0x0d4d, 0x0d15, 0x0d3e, 0x0d32, 0x0d02};
const char textMal[] = "കുട്ടിക്കാലം";
const char text[] = "Hello";

/* Replace this function with something useful. */

void
draw_bitmap( FT_Bitmap*  bitmap,
             FT_Int      x,
             FT_Int      y)
{
  FT_Int  i, j, p, q;
  FT_Int  x_max = x + bitmap->width;
  FT_Int  y_max = y + bitmap->rows;


  /* for simplicity, we assume that `bitmap->pixel_mode' */
  /* is `FT_PIXEL_MODE_GRAY' (i.e., not a bitmap font)   */

  for ( i = x, p = 0; i < x_max; i++, p++ )
  {
    for ( j = y, q = 0; j < y_max; j++, q++ )
    {
      if ( i < 0      || j < 0       ||
           i >= WIDTH || j >= HEIGHT )
        continue;

      image[j][i] |= bitmap->buffer[q * bitmap->width + p];
    }
  }
}

void
show_image( void )
{
  int  i, j;

  for ( i = 0; i < HEIGHT; i++ )
  {
    for ( j = 0; j < WIDTH; j++ )
    {
    	putpixel(j,i, image[i][j]==0?BLACK:/*image[i][j]<128?BLACK:*/RED);
    }
  }
}


// driver code
int main( int     argc,
      char**  argv )
{
    // gm is Graphics mode which is
    // a computer display mode that
    // generates image using pixels.
    // DETECT is a macro defined in
    // "graphics.h" header file
    int gd = DETECT, gm, color;
 
    // initgraph initializes the
    // graphics system by loading a
    // graphics driver from disk
    initgraph(&gd, &gm, "");

	FT_Library    library;
	FT_Face       face;

	FT_GlyphSlot  slot;
	FT_Matrix     matrix;                 /* transformation matrix */
	FT_Vector     pen;                    /* untransformed origin  */
	FT_Error      error;
	FT_Int glyph_count;

	char*         filename;
	char*         text;

	double        angle;
	int           target_height;
	int           n, num_chars;


	if ( argc != 3 )
	{
		fprintf ( stderr, "usage: %s font\n", argv[0] );
		exit( 1 );
	}

	filename      = argv[1];                           /* first argument     */
	text          = argv[2];                           /* second argument    */
	num_chars     = sizeof( text )/2;
	angle         = ( 0.0 / 360 ) * 3.14159 * 2;      /* use 25 degrees     */
	target_height = HEIGHT;



	hb_buffer_t *buf;
	buf = hb_buffer_create();
	hb_buffer_add_utf8(buf, text, -1, 0, -1);

	hb_buffer_set_direction(buf, HB_DIRECTION_LTR);
	hb_buffer_set_script(buf, HB_SCRIPT_MALAYALAM);
	hb_buffer_set_language(buf, hb_language_from_string("ml", -1));


	error = FT_Init_FreeType( &library );              /* initialize library */
	/* error handling omitted */

	error = FT_New_Face( library, filename, 0, &face );/* create face object */
	/* error handling omitted */

	/* use 50pt at 100dpi */
	error = FT_Set_Char_Size( face, 50 * 64, 0,
	                        100, 0 );                /* set character size */
	/* error handling omitted */

	/* cmap selection omitted;                                        */
	/* for simplicity we assume that the font contains a Unicode cmap */

	slot = face->glyph;

	/* set up matrix */
	matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
	matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
	matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
	matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );

	/* the pen position in 26.6 cartesian space coordinates; */
	/* start at (300,200) relative to the upper left corner  */
	pen.x = 50 * 64;
	pen.y = ( target_height - 50 ) * 64;

	hb_font_t *font = hb_ft_font_create_referenced(face);	

	hb_shape(font, buf, NULL, 0);
	hb_glyph_info_t *glyph_info    = hb_buffer_get_glyph_infos(buf, &glyph_count);

	for ( n = 0; n < glyph_count; n++ )
	{
		/* set transformation */
		FT_Set_Transform( face, &matrix, &pen );

		/* load glyph image into the slot (erase previous one) */
		error = FT_Load_Glyph( face, glyph_info[n].codepoint, FT_LOAD_RENDER );
		if ( error )
		  continue;                 /* ignore errors */

		/* now, draw to our target surface (convert position) */
		draw_bitmap( &slot->bitmap,
		             slot->bitmap_left,
		             target_height - slot->bitmap_top );

		/* increment pen position */
		pen.x += slot->advance.x;
		pen.y += slot->advance.y;
	}

	show_image();

	hb_buffer_destroy(buf);
	FT_Done_Face    ( face );
	FT_Done_FreeType( library );	

	getch();  

	// closegraph function closes the
	// graphics mode and deallocates
	// all memory allocated by
	// graphics system .
	closegraph();  

	return 0;    
}
