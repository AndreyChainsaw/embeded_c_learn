//https://cp-algorithms.com/string/string-hashing.html

long hashString(char *str) {
  long hash_value = 0;
  const int p = 31;
  long p_pow = 1;
  unsigned long m = 1 << 31; /* max 32b value */


  for (int i = 0; str[i] != 0; i++) {
    hash_value = (hash_value + (str[i] - 'a' + 1) * p_pow) % m;
    p_pow = (p_pow * p) % m;
  }

  return hash_value;
}
