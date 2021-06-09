# json2tlv

```bash
git clone git@github.com:yuriiz/json2tlv.git
cd json2tlv
make
./_builds/json2tlv<<EOF
{ "key1":"value", "key2":42, "key3":true, "key4": null}
EOF
xxd out.bin
```
