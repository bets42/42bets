<?xml version="1.0" encoding="UTF-8"?>
<soap:Envelope xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:n2="http://www.betfair.com/publicapi/types/global/v3/" xmlns:xsd="http://www.w3.org/2001/XMLSchema">
  <soap:Body>
    <n:logoutResponse xmlns:n="http://www.betfair.com/publicapi/v3/BFGlobalService/">
      <n:Result xsi:type="n2:LogoutResp">
        <header xsi:type="n2:APIResponseHeader">
          <errorCode xsi:type="n2:APIErrorEnum">OK</errorCode>
          <minorErrorCode xsi:nil="1"/>
          <sessionToken xsi:nil="1"/>
          <timestamp xsi:type="xsd:dateTime">2012-10-10T22:48:29.759Z</timestamp>
        </header>
        <minorErrorCode xsi:nil="1"/>
        <errorCode xsi:type="n2:LogoutErrorEnum">OK</errorCode>
      </n:Result>
    </n:logoutResponse>
  </soap:Body>
</soap:Envelope>
