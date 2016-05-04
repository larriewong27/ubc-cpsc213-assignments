package arch.sm213.machine.student;

import static org.junit.Assert.*;

import org.junit.Test;

import machine.AbstractMainMemory.InvalidAddressException;

public class MainMemoryTest {
    MainMemory mem=new MainMemory(10);
    
    @Test
    public void testSetAndGet() {
        byte[] data=new byte[3];
        data[0]=24;
        data[1]=69;
        data[2]=86;
        try {
                mem.set(4, data);
        } catch (InvalidAddressException e) {
                System.out.println("Invalid address, please try again.");
        }
        byte[] result=new byte[3];
        try {
                result=mem.get(4, 3);
        } catch (InvalidAddressException e) {
                System.out.println("Invalid address, please try again.");
        }
        assertArrayEquals(result, data);
    }
    
    @Test
    public void testIsAccessAlligned() {
        assertTrue(mem.isAccessAligned(4, 4));
    }
    
    @Test
    public void testIsAccessNotAlligned() {
        assertFalse(mem.isAccessAligned(3, 4));
    }
    
    @Test
    public void testIntegerToBytes() {
        byte[] bytesData = new byte[4];
        bytesData[0] = (byte) 0xEA;
        bytesData[1] = (byte) 0xFC;
        bytesData[2] = (byte) 0x44;
        bytesData[3] = (byte) 0x10;
        assertArrayEquals(bytesData, mem.integerToBytes(0xEAFC4410));
    }
    
    @Test
    public void testBytesToInteger() {
        assertEquals(0xBCCA1927, mem.bytesToInteger((byte) 0xBC, (byte) 0xCA, (byte) 0x19, (byte) 0x27));
    }
}
