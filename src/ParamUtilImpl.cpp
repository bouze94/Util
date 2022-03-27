#include <unordered_map>
#include <vector>
#include <string.h>
#include <memory>
#include <iostream>

union ParamData {
    bool* u8;
    int32_t* i32;
    int64_t* i64;
    float* f;
    double* d;
};

struct ParamData_t {
    int32_t count;
    ParamData data;
};

enum ParamType {
    PARAM_TYPE_START,
    BOOL,
    INT32,
    INT64,
    FLOAT,
    DOUBLE,
    PARAM_TYPE_END,
};

enum ParamTag {
    PARAM_TAG_START,
    BOUZE_INT32 = PARAM_TAG_START,
    BOUZE_BOOL,
    BOUZE_DOUBLE,
    BOUZE_FLOAT,
    PARAM_TAG_END,
};

static const ParamType ParamTagWithType[PARAM_TAG_END] = {
    INT32,
    BOOL,
    DOUBLE,
    FLOAT,
};

class  ParamUtil {
 public:
    ParamUtil() {
        for (int i = 0; i < PARAM_TAG_END; ++i) {
            m_storedParam[i].count = 0;
            switch (ParamTagWithType[i]) {
                case BOOL:
                    m_storedParam[i].data.u8 = nullptr;
                    break;

                case INT32:
                    m_storedParam[i].data.i32 = nullptr;
                    break;

                case INT64:
                    m_storedParam[i].data.i64 = nullptr;
                    break;

                case FLOAT:
                    m_storedParam[i].data.f = nullptr;
                    break;

                 case DOUBLE:
                    m_storedParam[i].data.d = nullptr;
                    break;

                 default:
                    break;
            }
        }
    }
    ~ParamUtil() {
        for (int i = 0; i < PARAM_TAG_END; ++i) {
            m_storedParam[i].count = 0;
            switch (ParamTagWithType[i]) {
                case BOOL:
                    delete[] m_storedParam[i].data.u8;
                    m_storedParam[i].data.u8 = nullptr;
                    break;

                case INT32:
                    delete[] m_storedParam[i].data.i32;
                    m_storedParam[i].data.i32 = nullptr;
                    break;

                case INT64:
                    delete[] m_storedParam[i].data.i64;
                    m_storedParam[i].data.i64 = nullptr;
                    break;

                case FLOAT:
                    delete[] m_storedParam[i].data.f;
                    m_storedParam[i].data.f = nullptr;
                    break;

                 case DOUBLE:
                    delete[] m_storedParam[i].data.d;
                    m_storedParam[i].data.d = nullptr;
                    break;

                 default:
                    break;
            }
        }
    }

    bool updateParam(int tag, void* data, size_t size) {
        bool ret = false;

        if (tag < PARAM_TAG_START || tag >= PARAM_TAG_END) return ret;

        if (m_storedParam[tag].count <= 0) {
            allocateParamBuf(tag, size);
        }

        switch (ParamTagWithType[tag]) {
            case BOOL:
                for (int i = 0; i < size; ++i) {
                    bool* tmpData = static_cast<bool*>(data);
                    m_storedParam[tag].data.u8[i] = tmpData[i];
                }
                ret = true;
                break;

            case INT32:
                for (int i = 0; i < size; ++i) {
                    int32_t* tmpData = static_cast<int32_t*>(data);
                    m_storedParam[tag].data.i32[i] = tmpData[i];
                }
                ret = true;
                break;

            case INT64:
                for (int i = 0; i < size; ++i) {
                    int64_t* tmpData = static_cast<int64_t*>(data);
                    m_storedParam[tag].data.i64[i] = tmpData[i];
                }
                ret = true;
                break;

            case FLOAT:
                for (int i = 0; i < size; ++i) {
                    float* tmpData = static_cast<float*>(data);
                    m_storedParam[tag].data.f[i] = tmpData[i];
                }
                ret = true;
                break;

                case DOUBLE:
                for (int i = 0; i < size; ++i) {
                    double* tmpData = static_cast<double*>(data);
                    m_storedParam[tag].data.d[i] = tmpData[i];
                }
                ret = true;
                break;

                default:
                break;
        }
        return ret;
    }

    bool allocateParamBuf(int tag, size_t size) {
        m_storedParam[tag].count = size;
        switch (ParamTagWithType[tag]) {
                case BOOL:
                    m_storedParam[tag].data.u8 = new bool[size];
                    break;

                case INT32:
                    m_storedParam[tag].data.i32 =  new int32_t[size];
                    break;

                case INT64:
                    m_storedParam[tag].data.i64 =  new int64_t[size];
                    break;

                case FLOAT:
                    m_storedParam[tag].data.f =  new float[size];
                    break;

                 case DOUBLE:
                    m_storedParam[tag].data.d =  new double[size];
                    break;

                 default:
                    break;
            }
    }

    ParamData_t findParam(int tag) {
        return m_storedParam[tag];
    }
private:
    std::unordered_map<int, ParamData_t> m_storedParam;
};

int main() {
    ParamUtil pu;
    int32_t tmpdata[3] = {1, 2, 3};
    pu.updateParam(BOUZE_INT32, tmpdata, 3);

    ParamData_t pd = pu.findParam(BOUZE_INT32);
    std::cout << "BOUZE_INT32 : ";
    if (pd.count > 0) {
        for (int i = 0; i < 3; ++i) {
            std::cout << pd.data.i32[i] << " ";
        }
    }
     std::cout << std::endl;


     bool tmpdata1[3] = {true, false, false};
    pu.updateParam(BOUZE_BOOL, tmpdata1, 3);
    pd = pu.findParam(BOUZE_BOOL);
    std::cout << "BOUZE_BOOL   : ";
    if (pd.count > 0) {
        for (int i = 0; i < 3; ++i) {
            std::cout << pd.data.u8[i] << " ";
        }
    }
     std::cout << std::endl;


    double tmpdata2[3] = {2.34545, 56.7778, 1212323.76576756};
    pu.updateParam(BOUZE_DOUBLE, tmpdata2, 3);
    pd = pu.findParam(BOUZE_DOUBLE);
    std::cout << "BOUZE_DOUBLE   : ";
    if (pd.count > 0) {
        for (int i = 0; i < 3; ++i) {
            std::cout << pd.data.d[i] << " ";
        }
    }
     std::cout << std::endl;

     
     float tmpdata3[3] = {2.12, 3.7778, 23.546};
    pu.updateParam(BOUZE_FLOAT, tmpdata3, 3);
    pd = pu.findParam(BOUZE_FLOAT);
    std::cout << "BOUZE_FLOAT   : ";
    if (pd.count > 0) {
        for (int i = 0; i < 3; ++i) {
            std::cout << pd.data.f[i] << " ";
        }
    }
     std::cout << std::endl;
}

