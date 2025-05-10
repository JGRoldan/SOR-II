<?php
/**
 * The base configuration for WordPress
 *
 * The wp-config.php creation script uses this file during the installation.
 * You don't have to use the website, you can copy this file to "wp-config.php"
 * and fill in the values.
 *
 * This file contains the following configurations:
 *
 * * Database settings
 * * Secret keys
 * * Database table prefix
 * * ABSPATH
 *
 * @link https://developer.wordpress.org/advanced-administration/wordpress/wp-config/
 *
 * @package WordPress
 */

// ** Database settings - You can get this info from your web host ** //
/** The name of the database for WordPress */
define( 'DB_NAME', 'dbprueba' );

/** Database username */
define( 'DB_USER', 'usuariotest' );

/** Database password */
define( 'DB_PASSWORD', 'Contraseña123!' );

/** Database hostname */
define( 'DB_HOST', 'localhost' );

/** Database charset to use in creating database tables. */
define( 'DB_CHARSET', 'utf8mb4' );

/** The database collate type. Don't change this if in doubt. */
define( 'DB_COLLATE', '' );

/**#@+
 * Authentication unique keys and salts.
 *
 * Change these to different unique phrases! You can generate these using
 * the {@link https://api.wordpress.org/secret-key/1.1/salt/ WordPress.org secret-key service}.
 *
 * You can change these at any point in time to invalidate all existing cookies.
 * This will force all users to have to log in again.
 *
 * @since 2.6.0
 */
define( 'AUTH_KEY',         '0%572K4`XJ@YAIE|W$f#p7$e27UEo&859s{1]%.w?{7/b(8@A/jHzFFA1qGy4Aks' );
define( 'SECURE_AUTH_KEY',  'gXUxm*G+=r!)C.]J3]6{>E .OKAqNcaQeW[PRiU< jp;-3>,>-&asdnWy|82mw.S' );
define( 'LOGGED_IN_KEY',    'Pw|,U#u4Fa{C`c+9=ZyZ.DV/c#go93y8ycY*h%xA{W.KdvDa) Wc*6-;^K=+Hw;U' );
define( 'NONCE_KEY',        'xb4`:`v9 #`,>GKLWsIa#Ha/a,|CxEQ|0-?x+vs IxyB?[N^nYL(a6;_dD)Y<u&m' );
define( 'AUTH_SALT',        'CL$LYz]Sp1:;MJisz3TQM~^)l(&U_hUGS-jR)VBw]W21;x]%gxF5=^K@5_L.^^fq' );
define( 'SECURE_AUTH_SALT', '0?i>H~0-w5Y v:yGE/T&,1?YRC i!n5VP=1sq&|gx|w[C1}tO:+98Ty-T7`Zgwc8' );
define( 'LOGGED_IN_SALT',   '#{@=O^)Iv?W4@;=k>hX#6$}Cg5w~D^=SO2*=]hyIg?;pyTk==N*kb8BV=,hP?i+M' );
define( 'NONCE_SALT',       '^CsCOTop-04RT2a)w}2l>`M>EW57p/IjeN3-LikN?$m&Lt@0ZFO|.M7Q nW!;dW`' );

/**#@-*/

/**
 * WordPress database table prefix.
 *
 * You can have multiple installations in one database if you give each
 * a unique prefix. Only numbers, letters, and underscores please!
 *
 * At the installation time, database tables are created with the specified prefix.
 * Changing this value after WordPress is installed will make your site think
 * it has not been installed.
 *
 * @link https://developer.wordpress.org/advanced-administration/wordpress/wp-config/#table-prefix
 */
$table_prefix = 'wp_';

/**
 * For developers: WordPress debugging mode.
 *
 * Change this to true to enable the display of notices during development.
 * It is strongly recommended that plugin and theme developers use WP_DEBUG
 * in their development environments.
 *
 * For information on other constants that can be used for debugging,
 * visit the documentation.
 *
 * @link https://developer.wordpress.org/advanced-administration/debug/debug-wordpress/
 */
define( 'WP_DEBUG', false );

/* Add any custom values between this line and the "stop editing" line. */



/* That's all, stop editing! Happy publishing. */

/** Absolute path to the WordPress directory. */
if ( ! defined( 'ABSPATH' ) ) {
	define( 'ABSPATH', __DIR__ . '/' );
}

/** Sets up WordPress vars and included files. */
require_once ABSPATH . 'wp-settings.php';

