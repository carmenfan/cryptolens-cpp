#include <iostream>

#include <curl/curl.h>

#include <cryptolens/core.hpp>
#include <cryptolens/Error.hpp>
#include <cryptolens/RequestHandler_curl.hpp>
#include <cryptolens/SignatureVerifier_OpenSSL.hpp>

namespace cryptolens = ::cryptolens_io::v20180502;
using Cryptolens = cryptolens::basic_SKM<cryptolens::RequestHandler_curl,cryptolens::SignatureVerifier_OpenSSL>;

/*
 * This example uses the basic_SKM class to make a request to the WebAPI
 * and then checks some properties of the license keys.
 */

void activate(char const* machine_code)
{
  Cryptolens cryptolens_handle;
  cryptolens::Error e;
  // Setting up the signature verifier with credentials from "Security Settings"
  // on serialkeymanager.com
  cryptolens_handle.signature_verifier.set_modulus_base64(e, "khbyu3/vAEBHi339fTuo2nUaQgSTBj0jvpt5xnLTTF35FLkGI+5Z3wiKfnvQiCLf+5s4r8JB/Uic/i6/iNjPMILlFeE0N6XZ+2pkgwRkfMOcx6eoewypTPUoPpzuAINJxJRpHym3V6ZJZ1UfYvzRcQBD/lBeAYrvhpCwukQMkGushKsOS6U+d+2C9ZNeP+U+uwuv/xu8YBCBAgGb8YdNojcGzM4SbCtwvJ0fuOfmCWZvUoiumfE4x7rAhp1pa9OEbUe0a5HL+1v7+JLBgkNZ7Z2biiHaM6za7GjHCXU8rojatEQER+MpgDuQV3ZPx8RKRdiJgPnz9ApBHFYDHLDzDw==");
  cryptolens_handle.signature_verifier.set_exponent_base64(e, "AQAB");

  cryptolens::optional<cryptolens::LicenseKey> license_key =
    cryptolens_handle.activate_floating
      ( // Object used for reporting if an error occured
        e
      , // SKM Access Token
        "WyIxNDQzIiwiZXBCVGc1S2tOVmRielVibXAxaWdtdVNtOXlGTnR0ekhkN2lwRnVKQiJd"
      , // Product id
        "3934"
      , // License Key
        "KRQNS-CSPNV-XRQBT-MTSBI"
      , // Machine Code
        machine_code
      , // Floating interval
        10
      );

  if (e) {
    // Error occured trying to activate the license key
    using namespace cryptolens::errors;

    if (e.get_subsystem() == Subsystem::Main) {
      // Handle errors from the SKM API
      std::cout << "SKM error: " << e.get_reason() << std::endl;
    } else if (e.get_subsystem() == Subsystem::RequestHandler && e.get_reason() == RequestHandler_curl::PERFORM) {
      int curlcode = e.get_extra();
      std::cout << "Error connecting to the server: curlcode: " << curlcode << std::endl;
    } else {
      std::cout << "Unhandled error: " << e.get_subsystem() << " " << e.get_reason() << " " << e.get_extra() << std::endl;
    }
    return;
  }

  std::cout << "Activated: " << machine_code << std::endl;
}

int main()
{
  curl_global_init(CURL_GLOBAL_SSL);

  activate("a");
  activate("b");
  activate("c");
  activate("d");
  activate("e");
  activate("f");
  activate("g");
  activate("h");
  activate("i");

  curl_global_cleanup();
}