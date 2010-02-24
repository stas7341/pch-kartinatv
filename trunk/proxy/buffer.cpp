#include <cstring>
#include <string>
#include <queue>

#define _MULTI_THREADED
#include <pthread.h>

using namespace std;

typedef unsigned char Byte;

#define BUF_SIZE (128*1024)

pthread_cond_t  rd_cond  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t rd_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t  wr_cond  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t wr_mutex = PTHREAD_MUTEX_INITIALIZER;


class Writer {
    Byte m_data[BUF_SIZE];

    unsigned m_read_idx;
    unsigned m_write_idx;

    void wakeup_reader() {
        pthread_cond_broadcast(&rd_cond);
    }

public:
    Writer() : m_write_idx(0)
    {}

    // set the read pointer to a new value
    void set_read_ptr(unsigned idx) { m_read_idx = idx; }

    // get the write pointer
    unsigned get_write_ptr() const { return m_write_idx; }

    const Byte *get_buffer() const { return m_data; }

    void enter(Byte *data, unsigned len) {
        while (len > 0) {
            unsigned free_size;
            if (m_read_idx <= m_write_idx) {
                // read index is behind us, write till end
                free_size = BUF_SIZE - m_write_idx;
                if (m_read_idx == 0) {
                    // do NOT reach it, this means empty
                    --free_size;
                }
            } else {
                free_size = m_read_idx - m_write_idx - 1;
            }
            if (free_size == 0) {
                // ring buffer is full, sleep and restart
                continue;
            }

            if (free_size >= len) {
                memcpy(m_data + m_write_idx, data, len);
                m_write_idx += len;
                m_write_idx %= BUF_SIZE;
                len = 0;
            } else {
                memcpy(m_data + m_write_idx, data, free_size);
                len -= free_size;
                m_write_idx = 0;
            }
            wakeup_reader();
        }
    }
};

class Reader {
    unsigned     m_read_idx;
    const Writer *m_writer;
    const Byte   *m_buffer;
    bool         m_sync;


    void wakeup_writer() {
        if (m_sync)
            pthread_cond_broadcast(&wr_cond);
    }

public:
    Reader(const Writer *writer, bool sync)
        : m_read_idx(0)
        , m_writer(writer)
        , m_buffer(writer->get_buffer())
        , m_sync(sync)
    {
    }

    unsigned read(Byte *data, unsigned len) {
        Byte *p = data;

        while (len > 0) {
            unsigned write_idx = m_writer->get_write_ptr();
            unsigned filled;
            if (m_read_idx < write_idx) {
                filled = write_idx - m_read_idx - 1;
            } else {
                filled = BUF_SIZE - m_read_idx;
            }
            if (filled == 0) {
                // sleep until data is filled in
                // aquiere mutes
                pthread_mutex_lock(&rd_mutex);
                // wait for cond var
                pthread_cond_wait(&rd_cond, &rd_mutex);
                continue;
            }

            if (filled > len)
                filled = len;
            memcpy(p, m_buffer + m_read_idx, filled);
            p += filled;
            m_read_idx += filled;
            m_read_idx %= BUF_SIZE;
            len -= filled;
            wakeup_writer();
        }
        return p - data;
    }
};

static string create_http_answer()
{
    string res;
    res += "HTTP/1.0 200 OK\r\n";
    res += "Content-type: application/octet-stream\r\n";
    res += "Cache-Control: no-cache\r\n";
    res += "\r\n";
    return res;
}

static string create_http_request(const string &path, const string &host_and_port) 
{
    string res;

    res += "GET " + path + " HTTP/1.0\r\n";
    res += "Host: " + host_and_port + "\r\n";
    res += "User-Agent: Mozilla/5.0\r\n";
    res += "Content-Type: application/x-www-form-urlencoded\r\n";
    res += "Connection: Close\r\n";
    res += "Accept: */*\r\n";
    res += "\r\n";
    return res;
}

struct Writer_args {
    Writer    *m_writer;
    pthread_t id;
};

static void* writer_thread(void *args)
{
    Writer_args *writer = reinterpret_cast<Writer_args *>(args);
    return writer;
}

struct Reader_args {
    Reader    *m_reader;
    pthread_t id;
};

static void* reader_thread(void *args)
{
    Reader_args *reader = reinterpret_cast<Reader_args *>(args);
    return reader;
}

int main(int args, char *argv[])
{
    queue<Reader*> readers;
    Writer_args    writer;
    bool first = true;

    if (first) {
        writer.m_writer = new Writer();
        pthread_create(&writer.id, NULL, writer_thread, (void*)&writer);

        Reader *reader    = new Reader(writer.m_writer, true);
        Reader_args *args = new Reader_args;

        readers.push(reader);
        pthread_create(&args->id, NULL, reader_thread, (void*)args);
    } else {
        Reader *reader    = new Reader(writer.m_writer, false);
        Reader_args *args = new Reader_args;

        readers.push(reader);
    }

    return 0;
}
