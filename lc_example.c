/**
 * Copyright (c) 2019 Sean Stasiak. All rights reserved.
 * Developed by: Sean Stasiak <sstasiak@protonmail.com>
 * Refer to license terms in license.txt; In the absence of such a file,
 * contact me at the above email address and I can provide you with one.
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <assert.h>

#define	PWM_PIN  (1)

PI_THREAD( pwm )
{
  /* YOU WILL NEED TO RUN AS SU TO GAIN
     ACCESS TO PIN IO! */


  /* GPIO1 is the PWM0 output pin */
  /* physical pin: 12             */
  pinMode( PWM_PIN, PWM_OUTPUT );
  pwmSetMode( PWM_MODE_MS );    /* mark-space pwm mode */

  /* brightness range is 0 -> 1023 (10 bits : 2^10)    */
  /* PWM period is 7*250us = 1.75ms, or rate of ~570Hz */
  while( ~0 )
  {
    uint32_t pwm_value = 0;
    for(; pwm_value <= 1023 ;pwm_value++)
    { /* up */
      pwmWrite( PWM_PIN, pwm_value );
      delay(5);
    }

    for(; pwm_value ;pwm_value--)
    { /* down */
      pwmWrite( PWM_PIN, pwm_value );
      delay(5);
    }
  }
}


/**
 * @internal
 * @brief rx packet buffer
 */
static char buf[64]; /**< DANGER - no buf ovf checks done! */

static int
    parse_rx_char( int c )
{
  enum{ F = 0, T = ~F };
  static int collecting = F;
  static char *pbuf = buf;

  int result = 0;

  switch( c )           /**< switch on 'special' flags in the input stream */
  {                     /*   regardless of current state                   */
    case '>' :
      collecting = T;
      pbuf = buf;     /**< unconditionally reset buffer!  */
      break;            /*   (re)starting a new transaction */
    case '<' :
      if( collecting )
      {
        collecting = F;
        *pbuf = 0x00;   /**< terminate as a normal c-string */
        pbuf = buf;
        result = ~0;    /**< signal pkt ready to caller     */
      }
      else { /* ignore - waiting for start of packet '>' */ }
      break;
    case -1:
      /* ERROR -> pass through */
      result = c;
      break;
    default :           /**< anything else depends on current rx state */
      if( collecting )
      {
        *pbuf++ = c;
      }
      else { /* ignore - waiting for start of packet '>' */ }
      break;
  }

  return result;
}

int main( void )
{
  if (wiringPiSetup() == -1)
  {
    fprintf( stderr, "wiringPiSetup() fail\r\n" );
    exit( ~0 );
  }

  piThreadCreate( pwm ) ;

  char const *const port = "/dev/ttyACM0";
  int const fd = serialOpen( port, 9600 );

  if( fd == -1 )
  {
    fprintf( stderr, "unable to open: %s\r\n", port );
    exit( ~0 );
  }

  /* in case the lc was left in a bad state, it doesn't
     hurt to 'reset' its parser */
  serialPuts( fd, ">>>>>>" );
  delay(100);
  serialFlush( fd );

  /* ALL LC COMMS ARE HALF DUPLEX - THIS MEANS
     ONLY ONE SIDE IS TX'ing AT A TIME */
  /* because of this constraint, we can share a
     tx/rx buffer in this example (buf[]) */

  while( ~0 )
  {
    /* current variable mappings:
        0 - food_temp_setpoint
        1 - p
        2 - i
        3 - d
        4 - loop_time
        5 - heat_element_temp
        6 - food_temp
    */







    /* for this example, we'll set the setpoint and
       fetch/confirm the set was performed */

    /* keep setpoint in a safe range in case we're
       running this test against real hardware */

    puts("\r\n\r\nfetch current setpoint...");
    serialPuts( fd, ">@|0:0<");                         /**< fetch food_temp_setpoint */
    while( parse_rx_char(serialGetchar(fd)) == 0 ) {}; /*< block until parser rx's a complete
                                                           pkt or an error occurs */
    /* NOTE: parse_rx_char() AUTOMATICALLY strips packet delimiters for us */
    printf( "raw response: >%s<\r\n", buf );

    static uint32_t setpoint = 0;
    setpoint = setpoint<40 ? setpoint+1 : 0;
    printf( "write new setpoint value: %u\r\n", setpoint );
    snprintf( buf, sizeof(buf), ">!|0:%u<", setpoint ); /**< store food_temp_setpoint */
    serialPuts( fd, buf );
    while( parse_rx_char(serialGetchar(fd)) == 0 ) {}; /*< block until parser rx's a complete
                                                           pkt or an error occurs */
    /* NOTE: parse_rx_char() AUTOMATICALLY strips packet delimiters for us */
    printf( "raw response: >%s<\r\n", buf );


    delay(1000);


    /* simple fetch example */
    puts("\r\n\r\nfetch food temp...");
    serialPuts( fd, ">@|6:0<" );  /**< fetch food_temp */
    while( parse_rx_char(serialGetchar(fd)) == 0 ) {}; /*< block until parser rx's a complete
                                                           pkt or an error occurs */
    /* NOTE: parse_rx_char() AUTOMATICALLY strips packet delimiters for us */
    printf( "raw response: >%s<\r\n", buf );
    uint32_t food_temp;
    if( sscanf( buf, "@|6:%u", &food_temp ) == EOF )
      puts( "parse failure!" );
    else
      printf( "parsed food temp: %u\r\n", food_temp );


    delay(1000);


  }

  serialClose( fd );
  return( 0 );
}