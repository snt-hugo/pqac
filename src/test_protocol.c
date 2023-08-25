#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>

#include "API.h"
#include "extra_tools.h"
#include "new_arithm_int.h"
#include "new_arithm_poly.h"
#include "params.h"
#include "protocol.h"
#include "samplers.h"
#include "shake.h"

#define PRINT_NAME_TEST  printf("**** %s ****\n" , __FUNCTION__)

int test_protocol();

int main()
{
  test_protocol();
  return EXIT_SUCCESS;
}

int test_protocol()
{
  PRINT_NAME_TEST;
  attribute_t attr = 4;
  message_t message;
  intq2_t intq2_id = {3, 0, 0};
  s_issuer_keys_t ikeys;
  s_opener_keys_t okeys;
  s_credential_t cred;
  s_presentation_t pres;
  printf("\nChoice of values : \n" );
  printf("Attribute set to %d\n", attr);
  printf("Identity set to ");
  intq2_print(intq2_id);
  printf("\nEach byte of the message is set to 1");

  protocol_init();
  intq2_set_ui(intq2_id, 3);
  for(int i = 0; i < SIZE_MESS_S8; i++)
    {
      message[i] = 1;
    }
  int result_presentation_verify;

  printf("\ninitialisation of issuer parameters");
  issuer_parameters_init(ikeys);
  printf("\ninitialisation of opener parameters");
  opener_parameters_init(okeys);

  printf("\ngeneration of issuer parameters");
  issuer_parameters_generation(ikeys);
  printf("\ngeneration of opener parameters");
  opener_parameters_generation(okeys);

  printf("\nInitialisation of the credential...");
  credential_init(cred);
  printf("\nGeneration of the credential...");
  credential_generation(cred, ikeys, intq2_id, attr);
  printf("\nVerification of the credential...\n" );
  if(credential_verify(cred, ikeys->pk, intq2_id, attr) != 0)
    {
      printf("SUCCESS : the verification of credential return 1");
    }
  else
    {
      printf("FAIL : the verification of credential doesn't return 1");
    }
  printf("\nInitialisation of presentation...");
  presentation_init(pres);
  printf("\nGeneration of presentation...");
  presentation_generation(pres, cred, intq2_id, ikeys->pk, okeys->pk, attr, message);
  uint8_t * compression_space = malloc(16000000 * sizeof(uint8_t));
  unsigned long long total_number_of_bits;
  total_number_of_bits = compression_presentation(compression_space, pres);
  printf("\nTotal number of bits used to encode the presentation token: %llu.\n",
         total_number_of_bits);
  s_presentation_t decompressed_presentation;
  presentation_init(decompressed_presentation);
  total_number_of_bits = decompression_presentation(decompressed_presentation, compression_space);
  printf("Total number of bits used to decode the byte string: %llu.\n", total_number_of_bits);
  printf("verification of presentation\n" );
  result_presentation_verify = presentation_verify(decompressed_presentation, ikeys->pk, okeys->pk,
                                                   attr, message);
  if(result_presentation_verify == 1)
    {
      printf("SUCCESS : the verification of presentation return 1");
    }
  else
    {
      printf("FAIL : the verification of presentation doesn't return 1");
    }
  free(compression_space);
  printf("\nclearing of  presentation");
  presentation_clear(pres);
  printf("\nclearing of issuer parameters");
  issuer_parameters_clear(ikeys);
  printf("\nclearing of opener parameters");
  opener_parameters_clear(okeys);
  printf("\nclearing of the credential\n");
  credential_clear(cred);
  protocol_clear();
  return EXIT_SUCCESS;
}
