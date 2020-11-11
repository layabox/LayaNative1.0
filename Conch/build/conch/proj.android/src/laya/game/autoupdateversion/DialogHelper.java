package laya.game.autoupdateversion;

import java.lang.reflect.Field;  

import android.app.AlertDialog;  
import android.app.AlertDialog.Builder;  
import android.content.Context;  
import android.content.DialogInterface;  
import android.content.DialogInterface.OnClickListener;  
import android.view.View;  
  
public class DialogHelper {  
  
    public static void Alert(Context ctx, CharSequence title, CharSequence message,  
            CharSequence okText, OnClickListener oklistener) {  
        AlertDialog.Builder builder = createDialog(ctx, title, message);  
        builder.setPositiveButton(okText, oklistener);  
        AlertDialog dlg = builder.create();
        dlg.setCanceledOnTouchOutside(false);	//防止点到外面自动关掉对话框。
        dlg.show();  
    }  
      
    public static void Alert(Context ctx, int titleId, int messageId,  
            int okTextId, OnClickListener oklistener) {  
        Alert(ctx, ctx.getText(titleId), ctx.getText(messageId), ctx.getText(okTextId), oklistener);  
    }  
  
    public static void Confirm(Context ctx, CharSequence title, CharSequence message,  
            CharSequence okText, OnClickListener oklistener, CharSequence cancelText,  
            OnClickListener cancellistener) {  
        AlertDialog.Builder builder = createDialog(ctx, title, message);  
        builder.setPositiveButton(okText, oklistener);  
        builder.setNegativeButton(cancelText, cancellistener);
        AlertDialog dlg = builder.create();
        dlg.setCanceledOnTouchOutside(false);	//防止点到外面自动关掉对话框。
        dlg.show();  
    }  
      
    public static void Confirm(Context ctx, int titleId, int messageId,  
            int okTextId, OnClickListener oklistener,  int cancelTextId,  
            OnClickListener cancellistener) {  
        Confirm(ctx, ctx.getText(titleId), ctx.getText(messageId), ctx.getText(okTextId), oklistener, ctx.getText(cancelTextId), cancellistener);  
    }  
  
    private static AlertDialog.Builder createDialog(Context ctx, CharSequence title,  
            CharSequence message) {  
        AlertDialog.Builder builder = new Builder(ctx);  
        builder.setMessage(message);  
        if(title!=null)  
        {  
            builder.setTitle(title);  
        }  
        return builder;  
    }      
      
    @SuppressWarnings("unused")  
    private static AlertDialog.Builder createDialog(Context ctx,int titleId, int messageId) {  
        AlertDialog.Builder builder = new Builder(ctx);  
        builder.setMessage(messageId);  
        builder.setTitle(titleId);  
        return builder;  
    }  
  
    public static void ViewDialog(Context ctx, CharSequence title, View view,  
            CharSequence okText, OnClickListener oklistener, CharSequence cancelText,  
            OnClickListener cancellistener) {  
  
    }  
      
    public static void ViewDialog(Context ctx, int titleId, View view,  
            int okTextId, OnClickListener oklistener, int cancelTextId,  
            OnClickListener cancellistener) {  
          
        ViewDialog(ctx, ctx.getText(titleId), view, ctx.getText(okTextId), oklistener, ctx.getText(cancelTextId), cancellistener);  
  
    }  
      
    //  
    public static void SetDialogShowing(DialogInterface dialog, boolean showing)  
    {  
        try {  
            Field field = dialog.getClass().getSuperclass().getDeclaredField("mShowing");  
            field.setAccessible(true);  
            field.set(dialog, showing);  
        } catch (Exception e) {  
            e.printStackTrace();  
        }  
    }  
  
}  