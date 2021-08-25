#pragma once

template<class T>
class CProxy_IATLNet6ObjectEvents :
    public ATL::IConnectionPointImpl<T, &__uuidof(_IATLNet6ObjectEvents)>
{
public:
    HRESULT  Fire_CalcResultChanged(LONG result)
    {
        HRESULT hr = S_OK;
        T* pT = static_cast<T*>(this);
        
        int nConnectionIndex; // index counter

        // NEEDED FOR VARIABLES 
        CComVariant* pvars = new CComVariant[1];

        // get number of objects that need to be notified
        int nConnections = this->m_vec.GetSize();
        
        // for every consumer that is connected to the connection point its event receiving function is called
        for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
        {
            pT->Lock();
            CComPtr<IUnknown> sp = this->m_vec.GetAt(nConnectionIndex);
            pT->Unlock();
            IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
            if (pDispatch != NULL)
            {
                pvars[0].vt = VT_I4;
                pvars[0].lVal = result;
                DISPPARAMS disp = { pvars, NULL, 1, 0 };
                pDispatch->Invoke(0x1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
            }
        }
        delete[] pvars;
        return hr;
    }
};