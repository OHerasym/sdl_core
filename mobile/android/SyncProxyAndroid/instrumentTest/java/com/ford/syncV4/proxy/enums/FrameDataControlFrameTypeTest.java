package com.ford.syncV4.proxy.enums;

import android.test.AndroidTestCase;

import com.ford.syncV4.protocol.enums.FrameDataControlFrameType;

import java.util.Arrays;

/**
 * Created by Andrew Batutin on 12/30/13.
 */
public class FrameDataControlFrameTypeTest extends AndroidTestCase {

    public void testFrameDataEndSession() throws Exception {
        assertNotNull("enum should have EndSession value", FrameDataControlFrameType.EndSession);
    }

    public void testFrameDataEndSessionACK() throws Exception {
        assertNotNull("enum should have EndSession value", FrameDataControlFrameType.EndSessionACK);
    }

    public void testEndSessionACKValueIs0x005() throws Exception {
        assertEquals("EndSessionACK should have 0x005 value", (byte) 0x05, FrameDataControlFrameType.EndSessionACK.getValue());
    }

    public void testGetValueReturnsEndSessionACK() throws Exception {
        assertEquals("value of EndSessionACK string should be FrameData.EndSessionACK", FrameDataControlFrameType.EndSessionACK, FrameDataControlFrameType.valueOf("EndSessionACK"));
    }

    public void testFrameDataValuesHasEndSessionACKValue() throws Exception {
        assertTrue("FrameData list should have EndSessionACK value", Arrays.asList(FrameDataControlFrameType.values()).contains(FrameDataControlFrameType.EndSessionACK));
    }
}
