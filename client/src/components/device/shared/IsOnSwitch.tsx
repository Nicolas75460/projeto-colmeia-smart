"use client"

import { useFormContext } from "react-hook-form"

import {
  FormControl,
  FormField,
  FormItem,
  FormLabel,
} from "@/components/ui/form"
import { Switch } from "@/components/ui/switch"

export function IsOnSwitch({ disabled }: { disabled?: boolean }) {
  const form = useFormContext();

  return (
          <div className="space-y-4">
            <FormField
              control={form.control}
              name="is_active"
              render={({ field }) => (
                <FormItem className="flex flex-row items-center gap-x-3">
                    <FormLabel>Ligado</FormLabel>
                  <FormControl>
                    <Switch
                      className="data-[state=checked]:bg-green-500"
                      disabled={disabled}
                      checked={field.value}
                      onCheckedChange={field.onChange}
                    />
                  </FormControl>
                </FormItem>
              )}
            />
          </div>
  )
}
