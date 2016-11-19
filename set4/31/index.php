<?php
/*
 * File name: index.php
 * Created by: WRH
 * Email: o@seceve.com
 * http://www.cryptopals.com/sets/4/challenges/31
 */

// write a tiny application that has a URL that takes a "file" argument and a "signature" argument, like so:
// http://localhost/?file=foo&signature=46b4ec586117154dacd49d664e5d63fdc88efb51
$key = 'a28v0xl2e8f2o7z0cne2e3'; // Signature key
$file_name = 'test.png'; // Limit $file_name for safety.

if( !isset( $_GET['file']) || !isset( $_GET['signature'])){
    header( 'Status Code: 500 Internal Server Error', true, 500);
    exit(0);
}

$filename = $_GET['file'];
$signature = $_GET['signature'];

if( $filename != $file_name || !is_file( $file_name)){
    header( 'Status Code: 500 Internal Server Error', true, 500);
    exit(0);
}

$f = fopen( $file_name, 'rb');
$str = fread( $f, filesize( $file_name));
fclose( $f);
$hmac = bin2hex( hash_hmac( "sha1", $str, $key, true));
// echo $hmac;
// Use your "insecure_compare" function to verify the HMACs on incoming requests, and test that the whole contraption works. Return a 500 if the MAC is invalid, and a 200 if it's OK.
insecure_compare( $signature, $hmac) ? 
    header( 'Status Code: 200 OK', true, 200) : 
    header( 'Status Code: 500 Internal Server Error', true, 500);


// Have the server generate an HMAC key, and then verify that the "signature" on incoming requests is valid for "file", using the "==" operator to compare the valid MAC for a file with the "signature" parameter (in other words, verify the HMAC the way any normal programmer would verify it).
// Write a function, call it "insecure_compare", that implements the == operation by doing byte-at-a-time comparisons with early exit (ie, return false at the first non-matching byte).
// In the loop for "insecure_compare", add a 50ms sleep (sleep 50ms after each byte).
function insecure_compare( $cmp_str0, $cmp_str1){
    for( $i = 0; $i < strlen($cmp_str0); $i++){
        if( $cmp_str0[$i] != $cmp_str1[$i]) return false;
        usleep(50000); // Sleep 50 ms
    }
    return strlen($cmp_str0) == strlen($cmp_str1);
}

?>