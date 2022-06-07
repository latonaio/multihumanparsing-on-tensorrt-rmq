#include <stdint-gcc.h>
#include <jansson.h>
#include "rmq-client/rabbitmq-client.h"
#include "rmq-client/rabbitmq-client.c"
#include <thread>
#include <chrono>
namespace myMetaData

{
    class MHPMetadata
    {
    public:
        static int Fnum;
        static rabbitmq_cli cli;

    public:
        void arrayPrint(char *str, int len)
        {
            int i = 0;
            for (; i < len; i++)
            {
                printf("%d", str[i]);
            }
            printf("\n");
            return;
        }

        void BuildJSON(std::uint8_t *classMap, const int sw, const int sh)
        {
 //           arrayPrint( (char *)classMap,160);
            json_t *root = json_object();
            json_error_t err;
            json_t *classify_row = json_array();
            json_t *classifies = json_array();

            int y = 0;
            for (; y < sh; y++)
            {
                json_t *rowdata = json_object();
                classify_row = json_array();
                int x = 0;
                for (; x < sw; x++)
                {
                    json_array_append(classify_row, json_integer(classMap[y * sw + x]));
                }

                json_object_set(rowdata, "y", json_integer(y));
                json_object_set(rowdata, "classes", classify_row);
                json_array_append(classifies, rowdata);
            }

            json_object_set(root, "fnum", json_integer(this->Fnum));
            json_object_set(root, "classify", classifies);
            char *strObj;
            strObj = json_dumps(root, 0);
            int reply = rabbitmq_cli_publish(this->cli, "mhp", strObj);
            // std::this_thread::sleep_for(std::chrono::milliseconds());
            this->Fnum++;
            return;
        };
    };

} // namespace myMetaData

rabbitmq_cli myMetaData::MHPMetadata::cli = new_rabbitmq_client("192.168.128.185", 32094, "AI", "guest", "guest");
int myMetaData::MHPMetadata::Fnum = 0;
